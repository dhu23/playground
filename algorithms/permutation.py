n = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

# nums is a list
def permutation(nums):

    if len(nums) == 1:
        return [nums]
    
    results = []
    for x in nums:
        l = list(nums)
        l.remove(x)
        
        subs = permutation(l)
        # print type(subs)
        for sub in subs:
            # print type(sub)
            # print sub+[x]
            results.append(sub+[x]) 
            # results.append([x]+sub)
    return results

def permutation_new(nums):
    pass


ans = permutation(n[:5])
print len(ans), ans
