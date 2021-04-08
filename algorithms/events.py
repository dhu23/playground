import random
import heapq
import itertools
import bisect

def build(events):
    time_points = itertools.chain.from_iterable(
            [[(s, 1), (e, -1)] for (s, e) in events])
    sorted_time_points = sorted(time_points, key=lambda x : x[0])
    print(sorted_time_points)
    
    times, counts = [], []
    for (t, count) in sorted_time_points:
        times.append(t)
        counts.append((counts[-1] if counts else 0)+count)
        assert counts[-1] >= 0

    return times, counts


def query(t, data):
    times, counts = data
    assert len(times) == len(counts)

    i = bisect.bisect(times, t)
    if i == 0:
        return 0
    else:
        return counts[i-1]

if __name__ == "__main__":
    events = [(15, 122), (16, 100), (55, 125), (1, 40)]
    print(build(events))
    
