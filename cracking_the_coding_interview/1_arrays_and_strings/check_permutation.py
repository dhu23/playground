# given two strings, check if one is a permutation of the other

import collections

def check_permutation_count(s1, s2):
    s1_len, s2_len = len(s1), len(s2)
    if s1_len != s2_len:
        return False
    return collections.Counter(s1) == collections.Counter(s2)

def check_permutation_radix(s1, s2):
    pass


if __name__ == "__main__":
    print(check_permutation_count('permutation', 'ermutationp'))
