from knight_move import add_edge


def build1():
    g = {}

    add_edge(g, 2, 0)
    add_edge(g, 0, 1)
    add_edge(g, 2, 3)

    #add_edge(g, 0, 2)
    #add_edge(g, 1, 2)
    #add_edge(g, 3, 3)

    return g, [0, 1, 2, 3]

def build2():
    g = {}

    for i in range(13):
        g[i] = []

    add_edge(g, 0, 1)
    add_edge(g, 0, 5)
    add_edge(g, 5, 4)
    add_edge(g, 4, 3)
    add_edge(g, 3, 5)
    add_edge(g, 4, 2)
    add_edge(g, 2, 3)
    add_edge(g, 3, 2)
    add_edge(g, 2, 0)

    add_edge(g, 6, 0)
    add_edge(g, 6, 4)
    add_edge(g, 6, 9)
    add_edge(g, 6, 8)
    add_edge(g, 8, 6)
    add_edge(g, 7, 6)
    add_edge(g, 7, 9)
    add_edge(g, 11, 4)
    add_edge(g, 11, 12)
    add_edge(g, 12, 9)
    add_edge(g, 9, 10)
    add_edge(g, 10, 12)
    add_edge(g, 9, 11)

    return g
    

# check cyclic. This works for digraph with one component. 
# with more than one, another path or recursion variable is needed
def cyclic_util(g, s, visited):
    if s not in g:
        return False

    visited.add(s)

    for v in g.get(s, []):
        if v not in visited:
            ret = cyclic_util(g, v, visited)
            if ret is True:
                return ret
        else:
            return True
    
    visited.remove(s)
    return False

def hamiltonian_util(g, vertices, s, visited):
    if s not in g:
        return

    visited.add(s)
    if len(visited) == len(vertices):
        print 'found hamilton path'
        return

    for v in g.get(s, []):
        if v not in visited:
            hamiltonian_util(g, vertices, v, visited)

    visited.remove(s)

# bfs to get the shortest path from s to s
def shortest_path(g, s):
    queue = [s]

    distance = {s: 0}
    visited = set()

    while queue:

        p = queue.pop(0)
        # print 'p=', p, 'to=', g.get(p, [])
        visited.add(p)


        for v in g.get(p, []):
            if v == s:
                return s, distance[p]+1
            
            if v not in visited:
                distance[v] = distance[p]+1
                queue.append(v)
            else:
                # print 'else case'
                distance[v] = min(distance[p]+1, distance[v])

    # print s, distance
    return s, distance[s]

def post_order_util(g, s, visited, order):
    visited.add(s)

    for v in g.get(s, []):
        if v not in visited:
            post_order_util(g, v, visited, order)
    order.append(s)

def topological_sort(g, vertices):
    visited = set()
    order = []
    for start in vertices:
        if start not in visited:
            post_order_util(g, start, visited, order)

    return order
    

def test1(g, vertices):   
    
    print cyclic_util(g, 0, set())
    hamiltonian_util(g, vertices, 0, set())


    print [shortest_path(g, start) for start in vertices]

    print topological_sort(g, vertices)
        

def reverse_graph(g):
    gr = {}
    for i in g.keys():
        gr[i] = []

    for f, adj in g.iteritems():
        for t in adj:
            # add t->f in gr
            gr[t].append(f)
    return gr

def post_order(g):
    visited = set()
    order = []
    print g.keys()
    for s in (g.keys()):
        if s not in visited:
            post_order_util(g, s, visited, order)
    return order

def korasaju_dfs(g, i, group, group_id):
    if i in group:
        return

    group[i] = group_id
    for v in g.get(i, []):
        if v not in group:
            korasaju_dfs(g, v, group, group_id)

def korasaju_sharir(g):
    group = {}

    group_id = 0
    for i in reversed(post_order(reverse_graph(g))):
        if i not in group:
            korasaju_dfs(g, i, group, group_id)           
            group_id += 1

    return group, group_id

if __name__ == "__main__":
    print '--------------------------test1-----------------------------'
    test1(*build1())

    print '--------------------------test1-----------------------------'
    g = build2()
    test1(g, g.keys())
    print g

    print 'reversed post order=', 
    print post_order(reverse_graph(g))
    
    import pprint
    #pprint.pprint(korasaju_sharir(g))

    #pprint.pprint(korasaju_sharir(reverse_graph(g)))
