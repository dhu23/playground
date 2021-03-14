 given a directed graph, design an algorithm to find out whether there is
# a route between two nodes

from graphs import *

def build():
    g = {}
    g[0] = [1, 4, 5]
    g[1] = [3, 4]
    g[2] = [1]
    g[3] = [2, 4]
    return g

def find_route(g, v1, v2):
    visited = set()
    dfs(g, v1, visited)
    return v2 in visited

if __name__ == "__main__":
    print(find_route(build(), 3, 5))
    print(find_route(build(), 3, 4))
