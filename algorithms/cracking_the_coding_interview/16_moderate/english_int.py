# given any integer, print an English phrase that describes the integer
# e.g. One Thousand, Two Hundred Thirty-Four

below_20 = ['Zero', 'One', 'Two', 'Three', 'Four', 'Five', 'Six', 'Seven',
            'Eight', 'Nine', 'Ten', 'Eleven', 'Twelve', 'Thirteen',
            'Fourteen', 'Fifteen', 'Sixteen', 'Seventeen', 'Eighteen',
            'Nineteen']
tens = [None, None, 'Twenty', 'Thirty', 'Fourty', 'Fifty', 'Sixty', 'Seventy',
        'Eighty', 'Ninety']

units = [None, 'Thousand', 'Million', 'Billion']

import random

def int_to_english(n):
    if n < 20: return below_20[n]
    if n < 100:
        return (tens[n//10] if n%10 == 0
                else tens[n//10] + '-' + below_20[n%10])
    if n < 1000:
        return (below_20[n//100] + ' Hundred' if n%100 == 0
                else below_20[n//100] + ' Hundred ' + int_to_english(n%100))

    # anything that's greater then 1000
    assert n < 1000000000000 # less than 1000 billion for testing purpose
    stack = []
    while n%1000 > 0:
        tail = n%1000
        n = n//1000
        stack.append(int_to_english(tail))

    #print(stack)
    assert len(stack) <= 3
    ret = []
    for i in reversed(range(1, len(stack))):
        #print(i)
        ret.append(stack[i] + ' ' + units[i])
    ret.append(stack[0])
    return ', '.join(ret)
        

if __name__ == "__main__":
    for i in range(1000):
        print(i, int_to_english(i))
                

    for i in (1234, 12345, 123456, 1234567, 12345678, 123456789):
        print(i, int_to_english(i))
