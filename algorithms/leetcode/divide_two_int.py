
def divide_by_subtraction(num, divisor):
    # build multiple of divisor
    multiple_data = [(0, 0, 0)]

    index, count, multiple = 1, 1, divisor
    multiple_data.append((index, count, divisor))
    
    while multiple < num:
        index += 1
        multiple += multiple
        count += count

        if multiple > num:
            break

        multiple_data.append((index, count, multiple))
        
    # print multiple_data

    quotient = 0
    remainder = num

    index = multiple_data[-1][0] # get the last index

    while remainder >= divisor:
        while remainder < multiple_data[index][2]:
            index -= 1
        remainder -= multiple_data[index][2]
        quotient += multiple_data[index][1]
        # print multiple_data[index]

    return quotient, remainder

print divide_by_subtraction(30, 2), divmod(30, 2)
print divide_by_subtraction(345, 23), divmod(345, 23)
print divide_by_subtraction(5678, 21), divmod(5678, 21)


