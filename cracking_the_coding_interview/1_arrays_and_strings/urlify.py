# write a method to replace all spaces in a string with '%20'
# you may assume the string has additional space at the end to hold the
# additional characters and you are given the true length of the string
# (if you are implementing in Java, use an array so the operation is in place)

def urlify(s, size):
    return s[:size].replace(' ', '%20')

if __name__ == "__main__":
    print(urlify('Mr John Smith    ', 13))
