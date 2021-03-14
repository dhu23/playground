# given a list of people with their birth and death years, implement a method
# to compute the year with the most number of people alive. You may assume
# that all people were born between 1900 and 2000(inclusive). If a person
# was alive during any portion of that year, they should be included in that
# year's count.

# for example:
# person (birth=1908, death=1909) is included in the counts for both 1908,1909

import random
import operator
import pprint

def get_sample():
    random.seed(100)
    bs = [random.randint(500, 600) for i in range(100)]
    ys = [random.randint(0, 100) for i in range(100)]
    return [(b, b+y) for b, y in zip(bs, ys)]
        
def get_most_living_year(dates):

    # build a stream of dates, mixed with both births and deaths with marks
    # more accurately, it is a stream of dates with the year that would
    # increase the living account and decrease the living accoutn, (b, d+1)

    stream = []
    for b, d in dates:
        stream.append((b, operator.add))
        stream.append((d+1, operator.sub))

    # NOTE: here you can use radix sort to essentially reduce the complexity
    # from O(NlogN) to O(N)
    stream.sort(key=operator.itemgetter(0))

    stack, max_living = [], 0
    for year, op in stream:
        print(year, op)
        if not stack: # op is operator.add
            assert op is operator.add
            stack.append([year, 1])
        # there is content in the stack
        else:
            count = stack[-1][1]
            if year == stack[-1][0]:
                stack[-1][1] = op(count, 1)
            else:
                assert year > stack[-1][0]
                stack.append([year, op(count, 1)])

        max_living  = max(max_living, stack[-1][1])
                
    return stack, max_living
            

if __name__ == "__main__":
    print(get_sample())
    print('--------------result------------')
    pprint.pprint(get_most_living_year(get_sample()))
