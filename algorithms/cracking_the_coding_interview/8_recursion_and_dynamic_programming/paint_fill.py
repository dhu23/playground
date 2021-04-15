# implement the paint fill function that one might see on many image editing
# programs. That is, given a screen (represented by a two-dimensional array
# of colors), a point, and a new color, fill in the surrounding area until
# the color changes from the original color. 

# this is essentially a BFS problem

# 'g' is green, 'b' is blue, 'r' is read
import random
import pprint

def build_2d_image(n, m):
    def get_random_row(j):
        return [random.choice('rg') for i in range(m)]
    random.seed(100)
    return list(map(get_random_row, range(n))), n, m


def paint_fill(p, n, m, x, y, original_color, replace_color):
    if x >= n or x < 0 or y >= m or y < 0: return
    if p[x][y] == original_color:
        p[x][y] = replace_color
        paint_fill(p, n, m, x+1, y, original_color, replace_color)
        paint_fill(p, n, m, x-1, y, original_color, replace_color)
        paint_fill(p, n, m, x, y+1, original_color, replace_color)
        paint_fill(p, n, m, x, y-1, original_color, replace_color)

def display(plot, original_color, replace_color):
    def replace(c):
        if c == original_color: return replace_color
        else: return c
        
    for row in plot:
        print(''.join(map(replace, row)))
        
if __name__ == "__main__":
    plot, n, m = build_2d_image(10, 15)
    pprint.pprint(plot)

    display(plot, 'r', ' ')

    paint_fill(plot, n, m, 4, 4, 'r', 'b')
    pprint.pprint(plot)

    display(plot, 'g', ' ')
