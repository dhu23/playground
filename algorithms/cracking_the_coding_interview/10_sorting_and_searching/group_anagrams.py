# write a method to sort an array of strings so that all the anagrams are
# next to each other
import collections

def least_anagram(s):
    chars = [None]*26
    for c in s:
        pos = ord(c)-ord('a')
        if chars[pos] is None:
            chars[pos] = 1
        else:
            chars[pos] += 1
    ret = []
    for i, each in enumerate(chars):
        if each is not None:
            letter = chr(ord('a')+i)
            ret.extend([letter]*each)
    return ''.join(ret)

def sort_anagram_naive(s_arr):
    return sorted(s_arr, key=least_anagram)
    

if __name__ == "__main__":
    data = ["cat", "deer", 'reed', 'reap', 'pear', 'boy']
    print(sort_anagram_naive(data))
