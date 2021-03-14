

# a board with a knight at 1. find all paths to 9 
#   1   2   3 
#   4   5   6 
#   7   8   9 
#       0

def add_edge(g, v1, v2):
    if v1 in g:
        if v2 not in g[v1]:
            g[v1].append(v2)
    else:
        g[v1] = [v2]

def build_graph():
    g = {}

    # from 1
    add_edge(g, 1, 6)
    add_edge(g, 1, 8)

    # from 2
    add_edge(g, 2, 7)
    add_edge(g, 2, 9)

    # from 3
    add_edge(g, 3, 4)
    add_edge(g, 3, 8)

    # from 4
    add_edge(g, 4, 3)
    add_edge(g, 4, 9)
    add_edge(g, 4, 0)

    # from 5

    # from 6
    add_edge(g, 6, 1)
    add_edge(g, 6, 7)
    add_edge(g, 6, 0)

    # from 7
    add_edge(g, 7, 2)
    add_edge(g, 7, 6)

    # from 8
    add_edge(g, 8, 1)
    add_edge(g, 8, 3)

    # from 9
    add_edge(g, 9, 4)
    add_edge(g, 9, 2)

    # from 0
    add_edge(g, 0, 4)
    add_edge(g, 0, 9)

    return g

def dfs(g, start, end, path):
    if start == end:
        print path

    if start not in g:
        raise Exception("%s not in graph" % start)

    for v in g[start]:
        if v not in path and v in g:
            path.append(v)
            dfs(g, v, end, path)
            path.pop(-1)


def dfs_stack(g, start):    
    '''dfs without using recursive function calls'''
    visited = set()
    visited.add(start)
    print 'dfs with stack'
    stack = [] 
    stack.append(start)
    while stack:
        node = stack.pop(-1)
        print node
        for v in g[node]:
            if v not in visited:
                stack.append(v)
                visited.add(v)

def bfs_queue(g, start):
    '''bfs with queue. it won't be done with a recursive method'''
    visited = set()
    visited.add(start)
    print 'bfs with queue'
    queue = []
    queue.append(start)
    while queue:
        node = queue.pop(0)
        print node
        for v in g[node]:
            if v not in visited:
                queue.append(v)
                visited.add(v)



if __name__ == "__main__":
    g = build_graph()
    dfs(g, 1, 9, [1])
    dfs_stack(g, 1)
    bfs_queue(g, 1)
