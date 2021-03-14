import itertools

# given a list of N strings with length L. find if there exists a pair of 
# distinct strings that are cyclic rotations of one another

radix = 'abcdefghijklmnopqrstuvwxyz'
radix_slots = dict([(letter, i) for i, letter in enumerate(radix)])

def get_data():
    return [
            'algorithms', 
            'structures',
            'binaryheap',
            'polynomial',
            'minimumcut',
            'digraphdfs',
            'sortsuffix',
            'suffixsort',
            'stringsort',
            'boyermoore',
            'stackstack',
            'digraphbfs'], 10

def verify(s, i):
    if i >= len(s) or i < 0:
        raise Exception("index %d out of boundary [0, %d]" % (i, len(s)))


def get_char(s, index, at):
    verify(s, index)
    verify(s, at)

    return s[(index+at) % len(s)]

def get_rotation_string(s, index):
    return ''.join([get_char(s, index, i) for i in range(len(s))])

def test_get_char():
    s = 'abcdefg'

    for i in range(len(s)):
        print get_rotation_string(s, i)

def lds_util(string_list, rotation_list, d):
    N = len(string_list)
    R = 26

    count_list = [0]*(R+1)
    
    for i, l in rotation_list: 
        c = get_char(string_list[i], l, d)
        slot = radix_slots[c]+1
        count_list[slot] += 1

    print count_list
    for i in range(1, R+1):
        count_list[i] += count_list[i-1]
    
    print count_list
    
    ret = [(None, None)]*len(rotation_list)
    for i, l in rotation_list:
        c = get_char(string_list[i], l, d)
        slot = radix_slots[c]
        ret[count_list[slot]] = (i, l)
        count_list[slot] += 1
    
    return ret

def lds(string_list, L):
    N = len(string_list)
    rotation_list = [
            each for each in itertools.product(range(N), range(L))]

    for d in range(L-1, -1, -1):
        rotation_list = lds_util(string_list, rotation_list, d)
    
    return rotation_list

if __name__ == "__main__":
    print radix_slots
    test_get_char()
    
    string_list, L = get_data()   
    ret = lds(string_list, L)

    for i, l in ret:
        print (i, l), get_rotation_string(string_list[i], l)

