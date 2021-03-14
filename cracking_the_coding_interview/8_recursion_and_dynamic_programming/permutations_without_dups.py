# write a method to compute all permutations of a string of unique characters

# use a recursive backtracking

# pseudocode
# boolean solve(Node n) {
#   if n is a leaf node {
#     if the leaf is a goal node, return true
#     else return false
#   } else {
#     for each child of n {
#       if solve(c) succeeds, return true
#     }
#     return false
#   }
# }

# an iterative method
# boolean solve(Node n) {
#   put node n on the stack;
#   while the stack is not empty {
#     if the node at the top of the stack is a leaf {
#       if it is a goal node, return true
#       else pop it off the stack
#     } else {
#       if the node at the top f the stack has untried children
#         push the next untried child onto the stack
#       else pop the node off the stack
#     }
#   }
#   return false
# }


def permutation(sl, start):

    if start == len(sl)-1: # a leaf node. And always a goal node
        print(sl)

    for i in range(start, len(sl)):
        sl[start], sl[i] = sl[i], sl[start]
        permutation(sl, start+1)
        sl[start], sl[i] = sl[i], sl[start]

if __name__ == "__main__":
    permutation(list('abc'), 0)
