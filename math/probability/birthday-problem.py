import random
import math
from collections import Counter

# use number 1- 365 to represent the 365 days a year
# for group size k, if there is no pair of persons with the same birthday,
# then, the probability would be
# p(no-pair) = Perm(365, k) / (365 ^ k)

def get_chance_of_same_birthday0(group_size):
    prob_no_pair = math.perm(365, group_size) / (365 ** group_size)
    return 1 - prob_no_pair


def get_chance_of_same_birthday(group_size):
    chance = 1.0
    n = 365
    while 365 - n + 1 <= group_size:
        chance *= n / 365
        n -= 1
    return 1 - chance


def get_random_group(group_size):
    bdays = Counter(random.randint(1, 364) for _ in range(group_size))
    most_common_bday, repeat = bdays.most_common(1)[0]
    return repeat > 1


def test_group_birthday(group_size, repeat=100):
    count = sum(int(get_random_group(group_size)) for _ in range(repeat))
    chance = count / repeat
    print(f'for group size {group_size}, chance is {chance}')


if __name__ == '__main__':
    test_group_birthday(14, 100)
    test_group_birthday(23, 100)
    test_group_birthday(35, 100)