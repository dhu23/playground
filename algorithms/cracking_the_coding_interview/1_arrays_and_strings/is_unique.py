# implement an algorithm to determine if a string has all unique characters
# What if you cannot use additional data structures?
import collections
def is_unique_naive(s):
    if not s:
        return True
    c = collections.Counter(s)
    return all(True if v == 1 else False for _, v in c.items())
    #return c.most_common()[0][1] == 1

def is_unique_sort(s):
    cs = sorted(list(s))
    for i in range(1, len(cs)):
        if cs[i] == cs[i-1]:
            return True
    else:
        return False
    

if __name__ == "__main__":
    print(is_unique_naive('implementation'))
    print(is_unique_sort('implementation'))
    print(is_unique_sort('is_unique'))
