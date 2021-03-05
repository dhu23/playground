#import xml.etree.ElementTree as ET
import sys
import string
import itertools
from collections import namedtuple

def _offset_by(offset, lead):
    def _inner(s):
        if lead is None: return ' '*offset + s
        if len(lead) > offset:
            raise Exception("Cannot do leading style for {0}".format(lead))
        return lead + ' '*(offset-len(lead)) + s
    return _inner

def _title_comment(s):
    if len(s) > 74:
        raise Exception("comment is too long {0}".format(s))
    slashes = '/' * ((78-len(s)) // 2)
    return slashes + ' ' + s + ' ' + slashes

def _unlines(lines, delim='\n'):
    return delim.join(lines)

def _ns(name, is_header): return '' if is_header else name + '::'

def _comma(is_header): return ';' if is_header else ''
        
def _make_field_name(tname):
    assert tname[0].isupper()
    return tname[0].lower() + tname[1:]

class IndentedStructure(object):
    def __init__(self, indent_size):
        self.indent_size = indent_size
        self.default_indent = _offset_by(self.indent_size, None)
        self.lead_comma_indent = _offset_by(self.indent_size, ',')
        self.includes_h = ['<iostream>']
        self.includes_cpp = []

class EnumType(IndentedStructure):
    def __init__(self, name, indent_size):
        super(EnumType, self).__init__(indent_size)
        self.name = name
        self.includes_h.append('<string>')
        self.includes_cpp.append('<unordered_map>')
        self.enum_vals = []
        self.use_default_val = True # use 0, 1, 2...instead of custom vals

    def add_enum(self, e, v):
        if not self.enum_vals:
            self.use_default_val = v is None
        else:
            if self.use_default_val != (v is None):
                raise Exception("Enum value type not consistent")
        # TODO add validation so e and v are both unique
        self.enum_vals.append((e, v))
        
    def _generate_enum_class_h(self):
        lines = ['enum class Value : int', '{']
        for i, (e, v) in enumerate(self.enum_vals):
            if v is None:
                line = '{0} = {1}'.format(e, i)
            else:
                line = '{0} = {1}'.format(e, v)

            _indent = self.default_indent if i == 0 else self.lead_comma_indent
            lines.append(_indent(line))
        lines.append('};')
        return lines
    
    def _gen_sig(self, is_header, line, **kwargs):
        return string.Template(line).substitute(
            ns=_ns(self.name, is_header), 
            **kwargs) + _comma(is_header)

    def _generate_from_int_sig(self, is_header):
        _sig = 'bool ${ns}fromInt(int in, ${name}::Value& out)'
        return self._gen_sig(
            is_header, 
            'static ' + _sig if is_header else _sig,
            name=self.name)

    def _generate_to_int_sig(self, is_header):
        _sig = 'int ${ns}toInt(${name}::Value in)'
        return self._gen_sig(
            is_header, 
            'static ' + _sig if is_header else _sig,
            name=self.name)

    def _generate_from_string_sig(self, is_header):
        _sig = 'bool ${ns}fromString(const std::string& in, ${name}::Value& out)'
        return self._gen_sig(
            is_header, 
            'static ' + _sig if is_header else _sig,
            name=self.name)

    def _generate_to_string_sig(self, is_header):
        _sig = 'std::string ${ns}toString(${name}::Value in)'
        return self._gen_sig(
            is_header, 
            'static ' + _sig if is_header else _sig,
            name=self.name)

    def _generate_size_sig(self, is_header):
        _sig = 'int ${ns}size()'
        return self._gen_sig(is_header, 'static ' + _sig if is_header else _sig) 

    def generate_h(self):
        lines = ['struct {0}'.format(self.name), '{']
        lines.extend(
            map(self.default_indent,
                self._generate_enum_class_h()))
        lines.extend(
            map(
                self.default_indent,
                [
                    self._generate_from_int_sig(True),
                    self._generate_to_int_sig(True),
                    self._generate_from_string_sig(True),
                    self._generate_to_string_sig(True),
                    self._generate_size_sig(True)
                ]
            )
        )
        lines.append('};')
        return lines

    def _generate_from_int_def(self):
        lines = [self._generate_from_int_sig(False), '{']
        lines.append(self.default_indent('switch(in)'))
        lines.append(self.default_indent('{'))
        for i, (e, v) in enumerate(self.enum_vals):
            line_format = 'case {0}: out = Value::{1}; return true;' 
            if v is None:
                line = line_format.format(i, e)
            else:
                line = line_format.format(v, e)
            lines.append(self.default_indent(line))
        lines.append(self.default_indent('default: return false;'))
        lines.append(self.default_indent('}'))
        lines.append('}')
        return lines

    def _generate_to_int_def(self):
        lines = [self._generate_to_int_sig(False), '{']
        lines.append(self.default_indent('switch(in)'))
        lines.append(self.default_indent('{'))
        for i, (e, v) in enumerate(self.enum_vals):
            line_format = 'case Value::{0}: return {1};'
            if v is None:
                line = line_format.format(e, i)
            else:
                line = line_format.format(e, v)
            lines.append(self.default_indent(line))
        lines.append(self.default_indent('default: return -1;'))
        lines.append(self.default_indent('}'))
        lines.append('}')
        return lines

    def _generate_from_string_def(self):
        lines = [self._generate_from_string_sig(False), '{']
        lines.append(self.default_indent('static const std::unordered_map<std::string, Value> m'))
        lines.append(self.default_indent('{'))
        for i, (e, _) in enumerate(self.enum_vals):
            _indent = self.default_indent if i == 0 else self.lead_comma_indent
            lines.append(self.default_indent(_indent('{{"{0}", Value::{0}}}'.format(e))))
        lines.append(self.default_indent('};'))
        lines.extend(
            map(
                self.default_indent, 
                [
                    'auto it = m.find(in);',
                    'if (it == m.end())',
                    '{',
                    self.default_indent('return false;'),
                    '}',
                    'out = it->second;',
                    'return true;'
                ]
            )
        )
        lines.append('}')
        return lines

    def _generate_to_string_def(self):
        lines = [self._generate_to_string_sig(False), '{']
        lines.append(self.default_indent('switch(in)'))
        lines.append(self.default_indent('{'))
        for e, _ in self.enum_vals:
            lines.append(self.default_indent('case Value::{0}: return "{0}";'.format(e)))
        lines.append(self.default_indent('default: return "";'))
        lines.append(self.default_indent('}'))
        lines.append('}')
        return lines

    def _generate_size_def(self):
        lines = [
            self._generate_size_sig(False), 
            '{', 
            self.default_indent('return {0};'.format(len(self.enum_vals))),
            '}'
        ]
        return lines

    def generate_cpp(self):
        lines = []
        lines.extend(map(lambda x: "#include " + x, self.includes_cpp))
        lines.extend(self._generate_from_int_def())
        lines.extend(self._generate_to_int_def())
        lines.extend(self._generate_from_string_def())
        lines.extend(self._generate_to_string_def())
        lines.extend(self._generate_size_def())
        return lines
    
    def generate_xml(self):
        lines = ['<EnumType name="{0}">'.format(self.name)]
        for e, v in self.enum_vals:
            if v is None:
                line = '<enum name="{0}" />'.format(e)
            else:
                line = '<enum name="{0}" value="{1}" />'.format(e, v)
            lines.append(self.default_indent(line))
    
        lines.append('<EnumType/>')
        return lines

DataType = namedtuple("DataType", ["tname", "includes"])

class SequenceType(IndentedStructure):
    def __init__(self, name, indent_size):
        super(SequenceType, self).__init__(indent_size)
        self.name = name
        self.data_members = []

    def add_data(self, d, t): # field name and type
        assert isinstance(t, DataType)
        # TODO add validation so that d is unique
        self.data_members.append((d, t))
        self.includes_h.extend(t.includes)

    def _generate_private_data(self):
        lines = []
        for d, t in self.data_members:
            lines.append('{0} {1}_;'.format(t.tname, d))
        return lines
            
    def _generate_accessors(self):
        lines = []
        for d, t in self.data_members:
            accessor_line = '{0}& {1}()'.format(t.tname, d)
            lines.append(accessor_line+';')
            lines.append('const '+accessor_line+' const;')
        return lines

    def generate_h(self):
        lines = ['class {0}'.format(self.name), '{']
        lines.extend(map(self.default_indent, self._generate_private_data()))
        lines.append('public:')
        lines.append(self.default_indent('{0}() {{}}'.format(self.name)))
        lines.extend(map(self.default_indent, self._generate_accessors()))
        lines.append(self.default_indent('std::ostream& print(std::ostream& os) const;'))
        lines.append('};')
        return lines
        
    def generate_inline_funcs(self):
        if not self.data_members:
            return []

        lines = [    
            _title_comment('inline functions for class {0}'.format(self.name)),
            ''
        ]
        for d, t in self.data_members:
            accessor_line = '{0}& {1}::{2}()'.format(t.tname, self.name, d)
            return_line = 'return {0}_;'.format(d)
            lines.extend([
                'inline',
                accessor_line,
                '{',
                self.default_indent(return_line),
                '}',
                '',
                'inline',
                'const ' + accessor_line + ' const',
                '{',
                self.default_indent(return_line),
                '}'
            ])
        return lines

class ChoiceType(IndentedStructure):
    def __init__(self, name, indent_size):
        super(ChoiceType, self).__init__(indent_size)
        self.name = name
        self.data_types = []

    def add_data(self, t):
        assert isinstance(t, DataType)
        self.data_types.append(t)
        self.includes_h.extend(t.includes)

    def _generate_union_data(self):
        lines = ['union U', '{']

        def _make_data(tname):
            return '{0} {1}_;'.format(tname, _make_field_name(tname))

        for t in self.data_types:
            lines.append(self.default_indent(_make_data(t.tname)))

        first_tname = self.data_types[0].tname
        lines.append(self.default_indent('U() {new(&%s_) %s();}' % (_make_field_name(first_tname), first_tname)))
        lines.append('};')
        lines.append('U data_;')
        lines.append('int type_;')
        return lines

    def _generate_type_check(self):
        lines = []
        for t in self.data_types:
            lines.append('bool is{0}() const;'.format(t.tname))
        return lines

    def _generate_make_type(self):
        lines = []
        for t in self.data_types:
            lines.append('{0}& make{0}();'.format(t.tname))
        return lines

    def _generate_accessors(self):
        lines = []
        for t in self.data_types:
            accessor_line = '{0}& {1}()'.format(t.tname, _make_field_name(t.tname))
            lines.append(accessor_line + ';')
            lines.append('const ' + accessor_line + ' const;')
        return lines

    def generate_inline_funcs(self):
        if not self.data_types:
            return []

        lines = [
            _title_comment('inline functions for class {0}'.format(self.name)),
            ''
        ]

        for t in self.data_types:
            accessor_line = '{0}& {1}::{2}()'.format(t.tname, self.name, _make_field_name(t.tname))
            return_line = 'return data_.{0}_;'.format(_make_field_name(t.tname))
            lines.extend([
                'inline',
                accessor_line,
                '{', 
                self.default_indent(return_line),
                '}',
                '',
                'inline',
                'const ' + accessor_line + 'const',
                '{',
                self.default_indent(return_line),
                '}',
            ])
        return lines

    def generate_h(self):
        lines = ['class {0}'.format(self.name), '{']
        lines.extend(map(self.default_indent, self._generate_union_data()))
        lines.append('public:')
        lines.append(self.default_indent('{0}();'.format(self.name)))
        lines.append(self.default_indent('void reset() {type_=-1;}'))
        lines.extend(map(self.default_indent, self._generate_type_check()))
        lines.extend(map(self.default_indent, self._generate_make_type()))
        lines.extend(map(self.default_indent, self._generate_accessors()))
        lines.append(self.default_indent('std::ostream& print(std::ostream& os) const;'))
        lines.append('};')
        return lines

    def _generate_ctor(self):
        lines = ['{0}::{0}()'.format(self.name)]
        lines.append(_offset_by(self.indent_size, ':')('data_()'))
        lines.append(self.lead_comma_indent('type_(-1)'))
        lines.extend(['{', '}'])
        return lines

    def _generate_type_check_def(self):
        lines = []
        for i, t in enumerate(self.data_types):
            lines.append('bool {0}::is{1}() const'.format(self.name, t.tname))
            lines.append('{')
            lines.append(self.default_indent('return type_ == {0};'.format(i)))
            lines.append('}')
            
        return lines
        
    def _generate_make_type_def(self):
        lines = []
        for i, t in enumerate(self.data_types):
            lines.append('{1}& {0}::make{1}()'.format(self.name, t.tname))
            lines.append('{')
            lines.extend(
                map(self.default_indent, 
                    [
                        'new (&data_.{0}_) {1};'.format(_make_field_name(t.tname), t.tname),
                        'type_ = {0};'.format(i),
                        'return data_.{0}_;'.format(_make_field_name(t.tname))
                    ])
            )
            lines.append('}')
        return lines

    def generate_cpp(self):
        lines = []
        lines.extend(self._generate_ctor())
        lines.extend(self._generate_type_check_def())
        lines.extend(self._generate_make_type_def())
        return lines

def generate_cfiles(data_type_list, file_name):

    assert '.' not in file_name
    file_name_h = file_name + ".h"
    file_name_cpp = file_name + ".cpp"

    def _mk_include_guard():
        return '_'.join(['_INCLUDED', file_name.capitalize(), 'H_'])

    def _get_includes():
        all_includes = set(itertools.chain.from_iterable([dt.includes_h for dt in data_type_list]))
        lines = []
        lines.extend(map(lambda x: "#include " + x, all_includes))
        return lines

    def _generate_h():
        lines = []
        include_guard = _mk_include_guard()
        lines.append("#ifndef " + include_guard)
        lines.append("#define " + include_guard)
        lines.extend(_get_includes())
        for dt in data_type_list:
            lines.extend(dt.generate_h())
        for dt in data_type_list:
            if not isinstance(dt, (SequenceType, ChoiceType)): continue
            lines.extend(dt.generate_inline_funcs())
        lines.append("#endif")
        return lines

    def _generate_cpp():
        lines = []
        lines.append('#include "{0}"'.format(file_name_h))
        for dt in data_type_list:
            if isinstance(dt, (SequenceType,)): continue
            lines.extend(dt.generate_cpp())
        return lines
        
    with open(file_name_h, 'w') as f:
        f.write(_unlines(_generate_h()))

    with open(file_name_cpp, 'w') as f:
        f.write(_unlines(_generate_cpp()))
        
        

if __name__ == "__main__":
    #tree = ET.parse(sys.argv[1])

    t1 = EnumType("Shift", 4)
    t1.add_enum("Parallel", None)
    t1.add_enum("Wave", None)
    t1.add_enum("Tent", None)
    print(_unlines(t1.generate_xml()))
    print(_unlines(t1.generate_h()))
    print(_unlines(t1.generate_cpp()))

    StringType = DataType("std::string", ['<string>'])
    IntType = DataType("int", [])
    BoolType = DataType("bool", [])

    pm = SequenceType("ParallelMethod", 4)
    wm = SequenceType("WaveMethod", 4)
    wm.add_data("left", IntType)
    wm.add_data("right", IntType)
    wm.add_data("upSlope", BoolType)
    tm = SequenceType("TentMethod", 4)
    tm.add_data("left", IntType)
    tm.add_data("mid", IntType)
    tm.add_data("right", IntType)

    print(_unlines(pm.generate_h()))
    print(_unlines(wm.generate_h()))
    print(_unlines(tm.generate_h()))

    ParallelMethodType = DataType("ParallelMethod", [])
    WaveMethodType = DataType("WaveMethod", [])
    TentMethodType = DataType("TentMethod", [])

    sm = ChoiceType("ShiftMethod", 4)
    sm.add_data(ParallelMethodType)
    sm.add_data(WaveMethodType)
    sm.add_data(TentMethodType)
    print(_unlines(sm.generate_h()))
    print(_unlines(sm.generate_inline_funcs()))
    print(_unlines(sm.generate_cpp()))

    generate_cfiles([t1, pm, wm, tm, sm], 'mtge_shift')
