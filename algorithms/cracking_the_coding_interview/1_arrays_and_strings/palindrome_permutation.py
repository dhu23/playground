# given a string, check if it is a permutation of a palindrome
# A palindrome is a word that is the same forwards and backwards. A
# permutatin is a rearrangement of all letters.

# example:
# input:    tact coa
# output:   True (permutation: "taco cat", "atco cta", etc)

import collections
def check_palindrome_permutation(s):
    c = collections.Counter(s)
    del c[' ']
    c_len = len(c)
    odds = sum(1 for _, i in c.most_common() if i%2 == 1)
    evens = sum(1 for _, i in c.most_common() if i%2 == 0)
    return (evens == c_len) or (evens == c_len-1 and odds == 1)

if __name__ == "__main__":
    print(check_palindrome_permutation('tact coa')) # true
    print(check_palindrome_permutation('tact toa')) # false
