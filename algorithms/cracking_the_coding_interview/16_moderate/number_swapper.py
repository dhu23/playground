# write a function to swap a number in place
# that is without temporary variables

if __name__ == "__main__":
    a, b = 2, 10
    print(a, b)

    # swap by adding
    a = a + b # a, b = 12, 10
    b = a - b # a, b = 12, 2
    a = a - b # a, b = 10, 2

    print(a, b)

    # swap by xor
    # a --> 1010
    # b --> 0010

    a = a ^ b # a, b = 1000, 0010
    b = a ^ b # a, b = 1000, 1010
    a = a ^ b # a, b = 0010, 1010

    print(a, b)

    
