# the longest path(diameter) and the center of all nodes(center)
# the center of all nodes, which minimizes the depths of all nodes, is not
# necessarily along the longest path


def add_edge0(g, a, b):
    adj_a = g.get(a, [])
    adj_a.append(b)
    g.update({a: adj_a})

def add_edge(g, a, b):
    add_edge0(g, a, b)
    add_edge0(g, b, a)

def build_undirected_acyclic_graph():
       
    g = {}

    add_edge(g, 0, 1)
    add_edge(g, 1, 6)
    add_edge(g, 1, 2)
    add_edge(g, 2, 3)
    add_edge(g, 2, 4)
    add_edge(g, 2, 5)
    add_edge(g, 0, 7)
    add_edge(g, 0, 8)
    add_edge(g, 8, 10)
    add_edge(g, 8, 9)
    add_edge(g, 8, 15)
    add_edge(g, 9, 11)
    add_edge(g, 11, 12)
    add_edge(g, 12, 14)
    add_edge(g, 12, 13)

    return g


def dfs(g, s, visited, depth, f):
    if s in visited:
        return 
    
    visited.update({s: (depth, f)})

    for t in g.get(s, []):
        if t not in visited:
            dfs(g, t, visited, depth+1, s)

def dfs_order(g, s):
    visited = {} # map of node to group
    dfs(g, s, visited, 0, None)

    return visited


def analyze(visited):
    deepest = []
    max_depth = None
    total_depth = 0
    for node, data in visited.iteritems():
        if max_depth is None or data[0] > max_depth:
            deepest = [node]
            max_depth = data[0]
        elif data[0] == max_depth:
            deepest.append(node)
    
        total_depth += data[0]

    return deepest, max_depth, total_depth


def get_longest_path():
    g = build_undirected_acyclic_graph()
    print g

    deepest, max_depth, _ = analyze(dfs_order(g, 0))
    
    
    visited = dfs_order(g, deepest[0])
    print visited

    deepest, max_depth, _ = analyze(visited)
    print deepest, max_depth

def brute_force_center():
    g = build_undirected_acyclic_graph()

    print [analyze(dfs_order(g, s))[1] for s in range(0, 16)]
    print [analyze(dfs_order(g, s))[2] for s in range(0, 16)]


if __name__ == "__main__":
    # get_longest_path()
    brute_force_center()
