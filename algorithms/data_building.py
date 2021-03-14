import random


def build_two_arrays_of_distinct_data():
    
    total_size = random.randint(100, 150) # total size N in [100, 150]

    one_size = random.randint(1, total_size-1) # array one size in [1, N-1]
    two_size = total_size-one_size

    total_data = random.sample(xrange(500), total_size)

    one_indices = random.sample(xrange(total_size-1), one_size)
    left = [total_data[l] for l in one_indices]
    right = [total_data[r] for r in xrange(total_size) if r not in one_indices]

    return left, right
