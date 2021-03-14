# fact a number
# print all the ways to do it.
# 24 = 2*12 = 2*2*6 = 2*2*2*3 = ...
# this is a backtracking algorithm


def factor_number(num, factors, factor=2):
    
    while factor**2 <= num:
        
        quotient, remainder = divmod(num, factor)
       
        if remainder == 0:
            factors.append(factor)

            # the following 3 lines are for printing
            factors.append(quotient)
            print factors
            factors.pop()

            factor_number(quotient, factors, factor)
            factors.pop()
            
        
        factor += 1

    return 

# recursion is probably easier to write but won't work all the time
# due to stack overflow on large number of recursion. Prefer loop based
# algorithm
def factor_number_non_recursive(num):
    stack = [num]
    work_num = num

    factor = 2

    def factor_it(num, factor=2):
        while factor*factor <= num:
            quotient, remainder = divmod(num, factor)
            if remainder == 0:
                return factor, quotient
            
            factor += 1
        return None, None
    
    while True:
        #print 'result stack current', stack
        work_num = stack[-1]
            
        factor, quotient = factor_it(work_num, factor)
        if quotient is not None:
            assert factor is not None
            stack.pop()
            stack.append(factor)
            stack.append(quotient)
            print stack # print a solution 
            work_num = quotient
        else:
            assert factor is None
            current_work = stack.pop()
            if not stack:
                return # end the whole process
            previous_factor = stack.pop()
            factor = previous_factor+1
            stack.append(previous_factor*current_work)
                  


if __name__ == "__main__":
    print '--------------------------'
    factor_number(24, [])
    print '--------------------------'
    factor_number(24*121, [])
    print '--------------------------'
    factor_number(6*5*4*3*2*1, [])
    print '--------------------------'
    factor_number(81*81, [])


    print '==========================='
    factor_number_non_recursive(24)
    print '==========================='
    factor_number_non_recursive (24*121)
    print '==========================='
    factor_number_non_recursive(6*5*4*3*2*1)
    print '==========================='
    factor_number_non_recursive(81*81)
