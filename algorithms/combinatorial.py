# n choose k + n choose k-1 = n+1 choose k
# calculate n choose k
import datetime

def measure_run_time(fn):
    def wrappee(*args, **kwargs):
        start = datetime.datetime.now()
        ret = fn(*args, **kwargs)
        print 'cost=%s, args=%s, kwargs=%s' % (
                datetime.datetime.now()-start, args, kwargs)
        return ret
    return wrappee

def combinatorial_recursive(n, k):
    if n < 0 or k < 0 or n < k:
        raise Exception("non-existing n=%d, k=%d" % (n, k))

    if k == 1:
        return n
    if k == 0:
        return 1
    if n == k:
        return 1

    return combinatorial_recursive(n-1, k) + combinatorial_recursive(n-1, k-1)

def combinatorial_dynamic(n, k, g):
    if n < 0 or k < 0 or n < k:
        raise Exception("non-existing n=%d, k=%d" % (n, k))

    if k == 1:
        g[(n, k)] = n
        return n
    if k == 0 or n == k:
        g[(n, k)] = 1
        return 1
    
    if (n, k) in g:
        return g[(n, k)]

    ret1 = g[(n-1, k)] if (n-1, k) in g \
            else combinatorial_dynamic(n-1, k, g) 
    ret2 = g[(n-1, k-1)] if (n-1, k-1) in g \
            else combinatorial_dynamic(n-1, k-1, g)
    
    g[(n, k)] = ret1+ret2
    return ret1+ret2

def combinatorial_buildup(n, k):
    results = [[1], [1, 1]] # 0 choose 0, 1 choose 0 and 1 choose 1

    for i in range(2, n):
        # corresponding to index i
        i_choose_list = []
        for j in range(0, i+1): # from 0 to i
            # corresponding to index j
            if i == j or j == 0:
                val = 1
            elif j == 1:
                val = i
            else:
                val = results[i-1][j]+results[i-1][j-1]
            i_choose_list.append(val)

        results.append(i_choose_list)
    
    return results[n-1][k]+results[n-1][k-1]

            

@measure_run_time
def test1(n, k):
    ret = combinatorial_recursive(n, k)
    print ret

@measure_run_time
def test2(n, k):
    ret = combinatorial_dynamic(n, k, {})
    print ret

@measure_run_time
def test3(n, k):
    ret = combinatorial_buildup(n, k)
    print ret

if __name__ == "__main__":
    #test1(30, 15)
    test2(30, 15)
    test3(30, 15)
    
    
