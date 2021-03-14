

from undirected_graph import add_edge

# Fleury's Algorithm for printing Eulerian Path or Circuit
# Make sure the graph has either zero or two odd vertices
# if there are no odd vertices, start anywhere, otherwise, start 
# at one of the odd vertices. 
# Follow edges one at a time, if you have a choice between a bridge and 
# a non-bridge, always choose the non-bridge.
# stop once you run out of edges

# bridge is a edge that the graph would be disconnected without

def build_graph1():
    '''
                       0
                      / \
                 1---2---3---4
                  \ /     \ /
                   5       6
                  / \     / \
                 7---8---9---10
                      \ /
                       11
    '''

    g = {}
    
    add_edge(g, 0, 2)
    add_edge(g, 0, 3)
    add_edge(g, 1, 2)
    add_edge(g, 2, 3)
    add_edge(g, 3, 4)
    add_edge(g, 1, 5)
    add_edge(g, 2, 5)
    add_edge(g, 3, 6)
    add_edge(g, 4, 6)
    add_edge(g, 5, 7)
    add_edge(g, 5, 8)
    add_edge(g, 6, 9)
    add_edge(g, 6, 10)
    add_edge(g, 7, 8)
    add_edge(g, 8, 9)
    add_edge(g, 9, 10)
    add_edge(g, 8, 11)
    add_edge(g, 9, 11)

    return g, 18


def build_graph2():
    '''
                     0---2
                     |  /|
                     | / |
                     |/  |
                     1   3
    '''
    g = {}
    
    add_edge(g, 0, 1)
    add_edge(g, 0, 2)
    add_edge(g, 1, 2)
    add_edge(g, 2, 3)
    
    return g, 4

def find_odd_vertex_count(g):
    odd_vertices = []
    for v, adj in g.iteritems():
        if len(adj) % 2 != 0:
            odd_vertices.append(v)
    return odd_vertices

def find_start_point(g):
    odd_vertices = find_odd_vertex_count(g)
    if len(odd_vertices) > 2:
        raise Exception(
                "more than 2 odd vertices, count=%d" % len(odd_vertices))
    elif len(odd_vertices) == 2:
        return odd_vertices[0]
    else: # when it is 0
        return 0

def dfs(g, s, visited):
    if s == -1:
        raise Exception("cannot dfs on -1 node")

    if s in visited:
        return 0

    dfs_count = 1
    visited.add(s)
    for v in g.get(s, []):
        if v not in visited and v != -1:
            dfs_count += dfs(g, v, visited)
    return dfs_count

def remove_edge(g, start, v):
    def remove_adj(g, f, t):
        if t in g[f]:
            g[f][g[f].index(t)] = -1
            
    remove_adj(g, start, v)
    remove_adj(g, v, start)

def is_valid_next_edge(g, start, v):    
    availables = [each for each in g.get(start, []) if each != -1]
    if len(availables) == 1:
        return True
    
    visited = set()
    reachable_count = dfs(g, v, visited)

    # try to remove v from start's adj list
    remove_edge(g, start, v)

    visited = set()
    reachable_count2 = dfs(g, v, visited)
    
    #print 'before=%d,after=%d,candidate=%d,from=%s' % (
    #        reachable_count, reachable_count2, v, start)        

    # add v back to adj list
    add_edge(g, start, v)
  
    return True if reachable_count == reachable_count2 else False


def euler_path_dfs(g, start, path, edge_count):
    if len(path) == edge_count:    
        print path
    
    availables = [each for each in g.get(start, []) if each != -1]
    # print 'availables=%s,from=%s' % (availables, start)    
        
    for v in availables:
        if is_valid_next_edge(g, start, v):

            remove_edge(g, start, v)
            
            path.append((start, v))
            euler_path_dfs(g, v, path, edge_count)
            path.pop(-1)
            
            add_edge(g, start, v)

def find_euler_path(g, edge_count):
    
    path = []
    euler_path_dfs(g, find_start_point(g), path, edge_count)


if __name__ == "__main__":
    g, edge_count = build_graph2()
    print g

    print find_start_point(g)

    visited = set()
    print dfs(g, 0, visited)


    find_euler_path(g, edge_count)
