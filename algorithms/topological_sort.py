from knight_move import add_edge
#                     1
#                    /
#                   /
#                  2
#                 /|\
#                / | \
#               3  |  \
#               |\ |   \
#               | \|    \
#               |  4---- 6
#               | /  
#               |/ 
#               5 
#               
#               
def build_graph():
    g = {}

    add_edge(g, 1, 2)
    add_edge(g, 2, 3)
    add_edge(g, 3, 4)
    add_edge(g, 2, 4)
    add_edge(g, 3, 5)
    add_edge(g, 4, 5)
    add_edge(g, 2, 6)
    add_edge(g, 4, 6)

    # to add a cycle
    # add_edge(g, 6, 1)
    
    return g

def dfs_stack(g, start):
    visited = set()

    path = []
    stack = [start]
    while stack:
        work = stack.pop(-1)
        visited.add(work)
        path.append(work)
        for v in g.get(work, []):
            if v not in visited:
                stack.append(v)
        # path.append(work)
    print path

def dfs(g, s, visited, topological_order):
    visited.add(s)

    for w in g.get(s, []):
        if w not in visited:
            dfs(g, w, visited, topological_order)
    # post order
    topological_order.append(s)

def dfs_order(g):
    visited = set()
    topological_order = []
    for s in [6, 3, 1, 4, 5, 2]:
        if s not in visited:
            print 'dfs for %d' % s
            dfs(g, s, visited, topological_order)
    print topological_order



if __name__ == "__main__":
    g = build_graph()
    dfs_stack(g, 2)
    dfs_order(g)
