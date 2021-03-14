# 123  --> 321
# -123 --> -321

def reverse_int(n):
    if n == 0:
        return n
    if n < 0:
        return -reverse_int(-n)
    if n < 10:
        return n

    ret = 0
 
    quotient, remainder = divmod(n, 10)
    # print quotient, remainder
    ret += remainder
    while quotient > 0:
        ret *= 10
        quotient, remainder = divmod(quotient, 10)
        # print quotient, remainder
        ret += remainder

    return ret

def check_palindrome_number(n):
    return reverse_int(n) == n

if __name__ == "__main__":
    print reverse_int(15)
    print reverse_int(12151316)
