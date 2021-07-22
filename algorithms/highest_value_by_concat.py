from functools import cmp_to_key

def comp(s1, s2):
    assert s1
    assert s2
    if s1[0] > s2[0]:
        return 1
    elif s1[0] < s2[0]:
        return -1
    else:
        head = s1[0]
        len1, len2 = len(s1), len(s2)
        maxlen = max(len1, len2)
        if maxlen <= 1:
            return 0

        idx = 1
        while True:
            val1 = s1[idx] if idx < len1 else head
            val2 = s2[idx] if idx < len2 else head

            break_flag = idx == maxlen-1

            if val1 > val2:
                return 1
            elif val1 < val2:
                return -1
            elif break_flag:
                return 0
            else:
                idx += 1
                continue


# n1=abc
# n2=defg
# compare abcdefg
#      vs defgabc

def concat_highest(ns):
    print(sorted(list(map(str, ns)), key=cmp_to_key(comp), reverse=True))


if __name__ == '__main__':
    concat_highest([9, 918, 917])
    concat_highest([918, 917, 9])
    concat_highest([1, 112, 113])
    concat_highest([112, 1, 113])
    concat_highest([557, 5, 6, 65, 678, 64, 661, 66])
