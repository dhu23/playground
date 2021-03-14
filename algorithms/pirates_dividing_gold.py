#!/usr/bin/python


goldBottles1 =\
        [20, 23, 1, 2, 30, 45, 19, 8, 56, 34, 25, 99, 29, 12, 15, 84, 25]
goldBottles2 =\
        [20, 23, 1, 2, 30, 45, 19, 8, 56, 34, 25, 99, 29, 12, 15, 84]
goldBottles3 =\
        [20, 23, 1, 2, 30, 45, 19, 8, 56, 34, 25, 99, 29, 12, 15]
goldBottles4 =\
        [20, 23, 1, 2, 30, 45, 19, 8, 56, 34, 25, 99, 29, 12]

operations = 0


def get_total_gold(data, left, right):
    if left >= right:
        # print 'take the last one', left, data[left]
        return data[left]

    # print operations
    
    index = left
    subTotal = 0
    while index <= right:
        global operations
        # print operations
        operations += 1
        subTotal += data[index]
        index += 1

    leftBottleValue = data[left]
    rightBottleValue = data[right]
    
    takeLeftTotalPayOff = subTotal-get_total_gold(data, left+1, right)
    takeRightTotalPayOff = subTotal-get_total_gold(data, left, right-1)

    if takeLeftTotalPayOff > takeRightTotalPayOff:
        # print 'take left', left, data[left]
        return takeLeftTotalPayOff
    else:
        # print 'take right', righot, data[right]
        return takeRightTotalPayOff


operations = 0
print get_total_gold(goldBottles1, 0, len(goldBottles1)-1)
print operations, len(goldBottles1), operations/(len(goldBottles1)**2)

operations = 0
print get_total_gold(goldBottles2, 0, len(goldBottles2)-1)
print operations, len(goldBottles2), operations/(len(goldBottles2)**2)

operations = 0
print get_total_gold(goldBottles3, 0, len(goldBottles3)-1)
print operations, len(goldBottles3), operations/(len(goldBottles3)**2)

operations = 0
print get_total_gold(goldBottles4, 0, len(goldBottles4)-1)
print operations, len(goldBottles4), operations/(len(goldBottles4)**2)
