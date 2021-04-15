# you are given a list of projects and a list of dependencies, which is a
# list of pairs of projects, where the second project is dependent on the
# first. All of a project's dependencies must be built before the project is
# Find a build order that will allow the projects to be built.
# If there is no valid build order, return an error 

# example
# input
# projects: a, b, c, d, e, f
# dependencies: (a, d), (f, b), (b, d), (f, a), (d, c)

# output: f, e, a, b, d, c


# this is a topological sort problem

def build_graph():
    nodes = ['a', 'b', 'c', 'd', 'e', 'f']
    g = {
        'a': ['d'],
        'f': ['b', 'a'],
        'b': ['d'],
        'd': ['c']
        }

    return g, nodes

# reversed post order traversal for topological sort
def build_order(g, nodes):

    def dfs(g, s, visited, post_order):
        visited.add(s)        
        if s in g:
            for v in g[s]:
                if v not in visited:
                    dfs(g, v, visited, post_order)
        post_order.append(s)

    post_order_for_sort = []
    visit_marks = set()
    for n in nodes:
        if n not in visit_marks:
            dfs(g, n, visit_marks, post_order_for_sort)
        print(n, post_order_for_sort)

    return list(reversed(post_order_for_sort))

# this starts with nodes that has no prerequisites, remove them from
# the dependency graph and rinse and repeat.
def get_nodes_without_prereq(g, nodes_set):
    nodes_with_prereq = set()
    for f, ts in g.items():
        for t in ts:
            nodes_with_prereq.add(t)

    # remove all edges starting from 
    nodes_without_prereq = nodes_set-nodes_with_prereq
    print(nodes_with_prereq, nodes_without_prereq)

    updated_g = dict(g)
    # remove nodes in nodes_without_prereq
    for f in nodes_without_prereq:
        if f in updated_g:
            del updated_g[f]
    return updated_g, nodes_with_prereq, list(nodes_without_prereq)
    
    

def build_order2(g, nodes):
    orders = []
    while nodes:
        g, nodes, removes = get_nodes_without_prereq(g, nodes)
        orders.extend(removes)
    print(orders)
    return orders
        


if __name__ == "__main__":
    print(build_order(*build_graph())) # prints f e b a d c


    print('================')
    g, nodes = build_graph()
    build_order2(g, set(nodes))
