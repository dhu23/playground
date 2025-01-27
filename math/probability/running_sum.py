# throw a fair die repeatedly in a sequence, track its running sum.
# for any given number N, you have a chance to see that in the running
# sequence if you throw the die enough times. 
# Calculate the probability

# as N -> inf, the probability -> 2/7

import random

def run_a_sequence(target) -> bool:
    if target < 0:
        return False
    elif target == 0:
        return True
    
    running_sum = 0
    while running_sum < target:
        throw = random.randint(1, 6)
        running_sum += throw
        if running_sum == target:
            return True
        elif running_sum > target:
            return False


def experiment(target, repeat=200):
    hit_count = 0
    for _ in range(repeat):
        if run_a_sequence(target):
            hit_count += 1
    print(
        f'target={target}, repeated={repeat}'
        f', hit={hit_count}, chance={hit_count/repeat}'
    )


if __name__ == '__main__':
    experiment(1)
    experiment(10)
    experiment(20)
    experiment(30)
    experiment(50)