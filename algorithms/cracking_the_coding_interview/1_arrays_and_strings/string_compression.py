# implement an algorithm to perform basic string compression using the counts
# of repeated characters. for example: aabcccccaaa would be a1b2c5a3
# if the compressed string would not become smaller than the original string,
# your method should return the original string
# assume it contains only upper case and lower case [a-z]

import itertools

def compress_string_functional(s):
    ret = []
    for c, g in itertools.groupby(s):
        ret.extend([c, str(len(list(g)))])
    ans = ''.join(ret)
    if len(ans) < len(s):
        return ans
    else:
        return s
        


if __name__ == "__main__":
    print(compress_string_functional('aabcccccaaa'))
