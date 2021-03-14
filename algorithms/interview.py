import string

def radix_sort0(input_list):
    counts = [0]*10
    for i in input_list:
        assert 0 <= i < 10
        counts[i] += 1
    # print 'c=', counts
    ret = []
    for i in range(10):
        ret.extend([i]*counts[i])

        # print ret
    return ret

def radix_sort(input_list, start, end, radix):
    counts = [0]*len(radix)
    for i in input_list[start:end+1]:
        assert i in radix
        counts[radix.index(i)] += 1
    ret = []
    for i in range(len(radix)):
        ret.extend([radix[i]]*counts[i])
    return ret

num_order = list('0123456789')
letter_order = list(string.ascii_uppercase)

def sort_alphanumeric(input_char_list):
    def check_letter(pos):
        print 'p=', pos
        # if pos >= len(input_char_list): return None
        return False if input_char_list[pos] in letter_order else True

    ret = []
    
    sub_str_start = 0
    is_number_substr = check_letter(sub_str_start)
    i, max_i = 1, len(input_char_list)-1
    print 'max-i=', max_i
    while True:
        print 'is num', is_number_substr
        while i <= max_i and check_letter(i) == is_number_substr:
            i += 1

        ret.extend(
            radix_sort(input_char_list,
                       sub_str_start,
                       i-1, num_order if is_number_substr else letter_order))
        print 'partial=',ret
        if i > max_i:
            break
        
        sub_str_start = i
        is_number_substr = check_letter(sub_str_start)
    
    return ret




if __name__ == "__main__":
    print radix_sort0([2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 1, 3, 4, 0, 1, 2, 3])

    #print num_order, letter_order
    #print radix_sort(list('1213513'), 0, 6, num_order)

    #print ''.join(sort_alphanumeric(list('AQF013452BAB')))
