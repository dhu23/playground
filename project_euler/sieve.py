
def mark_primes(limit):
    marks = [True]*limit
    marks[0], marks[1] = False, False

    for i in range(2, limit):
        if marks[i] is False:
            continue
        
        idx = i+i
        while idx < limit:
            marks[idx] = False
            idx += i
    return marks

def sum_primes(limit):
    return sum(i for i, is_prime in enumerate(mark_primes(limit)) if is_prime)

if __name__ == "__main__":

    #print(zip(enumerate(mark_primes(100))))
    print(sum_primes(10000))
