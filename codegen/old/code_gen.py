import pprint
example_schema_file = '''
{
    SCHEMA {
        RECORD TypeOne {
            STRING stringData;
            DOUBLE doubleData;
        }
        RECORD TypeTwo {
            STRING_ARRAY stringVecData;
            RECORD<TypeOne> typeOneData;
            RECORD_ARRAY<TypeOne> typeOneVecData;
        }
    }
}
'''

# type is tracked by the following data structure:
# 'data_map':
# {
#     'TypeOne': {
#         'fields': [
#             {'type': 'string', 'name': 'stringData', 'dim': 'single'},
#             {'type': 'double', 'name': 'doubleData', 'dim': 'single'},
#         ],
#     },
#     'TypeTwo': {
#         'fields': [
#             {'type': 'string', 'name': 'stringVecData', 'dim': 'vector'},
#             {'type': 'TypeOne', 'name': 'typeOneData', 'dim': 'single'},
#             {'type': 'TypeOne', 'name': 'typeOneVecData', 'dim': 'vector'},
#         ],
#     },
# },
# 'meta':
# {
#     'need_string': True, 'need_vector': True
# }
#

SIMPLE_TYPES = ('STRING', 'INT', 'DOUBLE')
SIMPLE_ARRAYS = ('STRING_ARRAY', 'INT_ARRAY', 'DOUBLE_ARRAY')
def parse_schema_file(schema):
    parsed = schema.split()

    data_map, need_string, need_vector = {}, False, False
    level = 0 # level is the indentation of the structure
    # when level = 1, it is schema level
    # when level = 2, it is type level
    # when level = 3, it is field level

    current_type = None
    current_field = None
    field_token_id = None
    
    for each in schema.split():
        if each == '{':
            level += 1
            if level == 3:
                field_token_id = 0
        elif each == 'SCHEMA':
            assert level == 1
        elif each == 'RECORD':
            assert level == 2
        elif each == '}':
            if level == 3:
                if field_token_id == 0:
                    field_token_id = None
                else:
                    raise Exception(
                        'error: field_token_id=%d' % field_token_id)
            level -= 1
        else:
            # encountering words
            if level == 2:
                # it is a new type name
                assert each not in data_map
                current_type = each
                data_map[each] = {}
            elif level == 3:
                # verify it is type toke
                token = each.upper()
                is_simple = token in SIMPLE_TYPES
                is_simple_array = token in SIMPLE_ARRAYS
                is_complex = each.startswith('RECORD<') and \
                             each.endswith('>')
                is_complex_array = each.startswith('RECORD_ARRAY<') and \
                                   each.endswith('>')

                need_string = need_string or (token == "STRING")
                need_vector = need_vector or is_simple_array or is_complex_array
                
                if is_simple or is_simple_array or \
                   is_complex or is_complex_array:

                    assert field_token_id == 0
                    if 'fields' not in data_map[current_type]:
                        data_map[current_type]['fields'] = []
                    fields = data_map[current_type]['fields']
                    fields.append({})

                    if is_simple:
                        type_string, dim_string = token, 'single'
                    elif is_simple_array:
                        type_string, dim_string = token[:-6], 'vector'
                    elif is_complex:
                        type_string, dim_string = each[7:-1], 'single'
                    else: # is_complex_array
                        type_string, dim_string = each[13:-1], 'vector'
                        
                    fields[-1]['type'] = type_string
                    fields[-1]['dim'] = dim_string
                  
                    fields[-1]['cpp_type'], \
                    fields[-1]['use_reference'] = get_cpp_type(fields[-1]) 
                            
                    field_token_id += 1
                elif each.endswith(';'):
                    assert field_token_id == 1
                    data_map[current_type]['fields'][-1]['name'] = each[:-1]
                    field_token_id -= 1    
            else:
                raise Exception('encountered type tokens at level=%d' % level)
    assert level == 0

    return {
        'data_map': data_map,
        'meta': {
            'need_string': need_string,
            'need_vector': need_vector}
        }

def indent(indent_num):
    def wrapped(func):
        def inner(*args, **kwargs):
            ret = func(*args, **kwargs)
            if isinstance(ret, str):
                return ' '*indent_num+ret
            elif isinstance(ret, list):
                return [(' '*indent_num+each) for each in ret]
            else:
                raise Exception('can only indent str or list')
        return inner
    return wrapped

def get_cpp_type(field_data_map):
    
    field_type, dim = field_data_map['type'], field_data_map['dim']
    
    if field_type.upper() == 'STRING':
        if dim == 'single':
            return "std::string", True
        elif dim == 'vector':
            return "std::vector< std::string >", True
        else:
            raise Exception('unrecognized dim=%s' % dim)
    elif field_type.upper() == 'INT':
        if dim == 'single':
            return 'int', False
        elif dim == 'vector':
            return 'std::vector< int >', True
        else:
            raise Exception('unrecognized dim=%s' % dim)
    elif field_type.upper() == "DOUBLE":
        if dim == 'single':
            return 'double', False
        elif dim == 'vector':
            return 'std::vector< double >', True
        else:
            raise Exception('unrecognized dim=%s' % dim)
    else:
        if dim == 'single':
            return field_type, True
        elif dim == 'vector':
            return 'std::vector< %s >' % field_type, True


def generate_class_declaration_body_lines(type_name, type_data):
    lines = ['', 'class %s' % type_name, '{']

    # add private data member sector
    for field in type_data['fields']:
        lines.append(generate_data_member_line(field))
        #lines.append('    %s' % generate_data_member_line(field))
    lines.append('')
    lines.append('public:')

    # add public data accessor sector
    for field in type_data['fields']:
        lines.extend(generate_accessor_lines(field))
        lines.append('')

    # add constructors and assignement operator
    lines.extend(generate_constructors(type_name))
    lines.append('')
    
    # add print function
    lines.append(generate_print_function())
    
    lines.append('};')
    print '\n'.join(lines)

@indent(4)
def generate_data_member_line(field_map):
    return '%s _%s;' % (field_map['cpp_type'], field_map['name'])

@indent(4)
def generate_accessor_lines(field_map):
    return [
        '%s& %s();' % (field_map['cpp_type'], field_map['name']),
        'const %s& %s() const;' % (field_map['cpp_type'], field_map['name'])]


@indent(4)
def generate_constructors(type_name):
    return [
        #'%s() {}' % type_name,
        #'%s(const %s& other);' % (type_name, type_name),
        #'%s& operator=(const %s& other);' % (type_name, type_name)
    ]

@indent(4)
def generate_print_function():
    return 'std::ostream& print(std::ostream&) const;'


def generate_class_inline_accessors(type_name, type_data):
    lines = []
    for each in type_data['fields']:
        lines.extend(generate_class_inline_field_accessor(type_name, each))
    print '\n'.join(lines)
    

def generate_class_inline_field_accessor(type_name, field):

    return [
        'inline',
        '%s& %s::%s()' % (
            field['cpp_type'], type_name, field['name']),
        '{',
        '    return _%s;' % field['name'],
        '}',
        '',
        'inline',
        'const %s& %s::%s() const' % (
            field['cpp_type'], type_name, field['name']),
        '{',
        '    return _%s;' % field['name'],
        '}',
        '']

def generate_free_functions(type_name):
    print 'std::ostream& operator<<(std::ostream& os, const %s& rhs);' % (
        type_name)

    
def generate_h_file(data_map):

    # generate class declarations
    for type_name in data_map:
        type_data = data_map[type_name]
        generate_class_declaration_body_lines(type_name, type_data)
        
    # generate inline functions for accessors
    for type_name in data_map:
        type_data = data_map[type_name]
        generate_class_inline_accessors(type_name, type_data)
        
    # generate free functions 
    # for type_name in data_map:
    #     generate_free_functions(type_name)

def generate_cpp_file(data_map):
    
    # generate accessors

    # generate print functions
    for type_name in data_map:
        type_data = data_map[type_name]
        generate_print_definition(type_name, type_data['fields'])

def generate_print_definition(type_name, fields):
    lines = [
        'std::ostream& %s::print(std::ostream& os) const' % type_name,
        '{',
        '    os << "["']
        
if __name__ == "__main__":
    schema_data = parse_schema_file(example_schema_file)

    pprint.pprint(schema_data)

    generate_h_file(schema_data['data_map'])
