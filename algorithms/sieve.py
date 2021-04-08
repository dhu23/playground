import itertools
import datetime

def make_primes(n):
    marks = [True]*n

    def mark_all(start, marks):
        i = start+start
        while i < n:
            marks[i] = False
            i += start

    marks[0], marks[1] = False, False

    start = 2
    while start < n:
        mark_all(start, marks)
        start += 1
    
    return marks

def sum_primes(n):
    return sum(i for i, is_prime in enumerate(make_primes(n)) if is_prime)

def get_primes(n):
    return [i for i, is_prime in enumerate(make_primes(n)) if is_prime]

if __name__ == "__main__":
    start_t = datetime.datetime.now()
    print list(enumerate(make_primes(1000)))
    # print sum_primes(2000000)
    print get_primes(1500)
    print datetime.datetime.now() - start_t
