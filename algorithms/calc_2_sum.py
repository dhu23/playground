import sys
l = [3, 4, 5, 8, 19, 21, 22, 23, 30, 35, 36, 56, 59, 63]
# l = range(50)
SUM = 37

lo = 0
hi = len(l)-1

min_val = None
min_pair = (None, None)
while lo < hi:
    
    while l[lo]+l[hi] < SUM and lo < hi:
        val = abs(SUM-l[lo]-l[hi])
        if min_val is None or min_val > val:
            min_val = val
            min_pair = (l[lo], l[hi])
        lo += 1

    while l[lo]+l[hi] > SUM and lo < hi:
        val = abs(SUM-l[lo]-l[hi])
        if min_val is None or min_val > val:
            min_val = val
            min_pair = (l[lo], l[hi])
        hi -= 1

    if l[lo]+l[hi] == SUM and lo < hi:

        print l[lo], l[hi]

    lo += 1
print min_val, min_pair


brute_force_result = (None, None)
closest = None
for i in l:
    for j in l:
        if j <= i: continue
        val = abs(SUM-i-j)
        if closest is None or closest > val:
            brute_force_result = (i, j)
            closest = val

print brute_force_result, closest
