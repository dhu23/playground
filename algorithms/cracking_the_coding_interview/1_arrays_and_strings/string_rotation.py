# assume that you have a method isSubstring which checks if one word is a
# substring of another. Given two strings, s1 and s2, write code to check if
# s2 is a rotation of s1 using only one call to isSubstring
# e.g. 'waterbottle' is a rotation of 'erbottlewat'

def is_substring(s, sub):
    return sub in s

def check_string_rotation(s1, s2):
    s = s1+s1
    return is_substring(s, s2)

if __name__ == "__main__":
    print(check_string_rotation('waterbottle', 'erbottlewat'))
