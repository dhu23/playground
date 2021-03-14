# given a string containing just the character ( and )
# find the length of the longest valid well formed parentheses substring
# (() --> 2
# )()()) --> 4

# dynamic programming
# for a substring of length l, s[i:i+l]
def longest_valid_parenthese_naive(s):
    # init P function map, which indicates whether if the substring is valid
    p_map = []
    s_len = len(s)
    for i in range(s_len):
        row = [True if l == 0 else False for l in range(s_len-i)]
        p_map.append(row)

    longest = (None, None)
    # calculate P function map
    for l in range(2, s_len):
        for i in range(s_len-l):
            sub_s = s[i:i+l]
            if sub_s[0] != '(' or sub_s[-1] != ')':
                continue

            if p_map[i+1][l-2] is True:
                p_map[i][l] = True
            else:
                for k in range(1, l):
                    if p_map[i][k] is True and p_map[i+k][l-k] is True:
                        p_map[i][l] = True
                        break
            if p_map[i][l] is True:
                # update longest
                longest_i, longest_l = longest
                if longest_i is None or longest_l < l:
                    longest = (i, l)



    print(p_map)
    return longest

# this is very similar to the problem that converts odem to python dict
# ((())()()(())((((((()(
# (            ((((((  (

# (()()))))))))
#       )))))))

def longest_valid_parenthese_stack(s):
    stack = []  # a stack that tracks indices of '('
    s_len = len(s)
    for i in range(s_len):
        # if it is '(', add its position to the stack
        # if it is ')', pop the top off the stack
        # if stack is empty already, append it
        if s[i] == ')' and stack and s[stack[-1]] == '(':
            stack.pop()
        else:
            stack.append(i)
    print(stack)

    # count holes in the stack
    s_i = 0
    stack_i = 0
    hole_stack, hole_start = [], None
    while s_i < s_len and stack_i < len(stack):
        if s_i == stack[stack_i]:
            if hole_start is not None:
                hole_stack.append((hole_start, s_i))
                hole_start = None
            s_i += 1
            stack_i += 1
        else:
            assert s_i < stack[stack_i]
            if hole_start is None:
                hole_start = s_i
            s_i += 1
    print(hole_stack)

    # get the longest interval
    print(max(hole_stack, key=lambda x: x[1]-x[0]))
    


def longest_valid_parenthese_stack2(s):
    stack = []
    s_len = len(s)

    push_to_stack = False
    for i in range(s_len):
        print(stack)
        if push_to_stack:
            stack.append((1, i))
            # collapse stack on ')'
            if s[i] == ')':
                # go back to find something like (1, j) where s[j] == '('
                accumulate = 1
                stack.pop()
                
                while stack:
                    length, pos = stack[-1]
                    if length != 1 or s[pos] != '(':
                        stack.pop()
                        accumulate += length
                    else:
                        stack.pop()
                        accumulate += length
                        stack.append((accumulate, None))
                        break

                if not stack:
                    push_to_back = False
                    
        else:
            # meaning we haven't seen ( yet
            if s[i] == ')':
                # discard
                continue
            else:
                push_to_stack = True
                stack.append((1, i))

    print(stack)
    
if __name__ == "__main__":
    #print(longest_valid_parenthese_naive(")()())"))
    #longest_valid_parenthese_stack2(")()())")
    longest_valid_parenthese_stack(")()())")
    longest_valid_parenthese_stack("(((()()()(()()")
    longest_valid_parenthese_stack("(()()((()))()()())))))))()())())")
