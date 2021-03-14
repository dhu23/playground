# imagine a robot sitting on the upper left corner of a grid with r rows and
# c columns. THe robot can only move in two directions: right and down, but
# certain cells are "off limits" such that the robot cannot step on them.
# Design an algorithm to find a path for the robot from the top left to the
# bottom right.


import pprint

# return a grid looking like the following
# R...O...
# .OO..O..
# ..O....O
# ....O...
def build_grid():
    n, m = 4, 8
    grid = [
        list('R...O...'),
        list('.OO..O..'),
        list('..O....O'),
        list('....O...'),
        ]
    assert len(grid) == n and len(grid[0]) == m
    assert grid[0][0] == 'R'
    pprint.pprint(grid)
    return grid, n, m

def find_path(grid, n, m):
    path_table = [[None]*m for i in range(n)]

    def within_grid(p):
        x, y = p
        return x >=0 and x < n and y >= 0 and y < m

    # once a node is in the q, it is considered 'visited' already
    visited = set([(0, 0)])
    q = [(0, 0)]
    while q:
        x, y = q.pop(0)
        for px, py in filter(within_grid, [(x+1, y), (x, y+1)]):
            if (px, py) in visited: continue
            if grid[px][py] == 'O': continue
            if path_table[px][py] is None:
                path_table[px][py] = []
            path_table[px][py].append((x, y))
            q.append((px, py))
            visited.add((px, py))
            #print(px, py, path_table)
      
    pprint.pprint(path_table)
    return path_table


def print_path(path_table, start, end):
    n = end
    path = [end]
    while path_table[n[0]][n[1]]:
        path.append(path_table[n[0]][n[1]][0])
        n = path_table[n[0]][n[1]][0]

    if n == start:
        return path
    return []

if __name__ == "__main__":
    grid, n, m = build_grid()

    pt = find_path(grid, n, m)
    print(print_path(pt, (0, 0), (3, 7)))
    print(print_path(pt, (0, 7), (3, 7)))
