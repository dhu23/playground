# A shape : a a a a a
# B shape : b b b b
#                 b
# C shape : c c c c
#               c
# D shape : d
#           d d d
#               d
# E shape : e
#           e
#           e e e
# F shape : f   f
#           f f f
# G shape :   g
#           g g g
#             g
# H shape : h h h
#             h
#             h
# I shape : i i
#             i i
#               i
# J shape : j j
#           j j j
# K shape : k k
#             k k k
# L shape : l
#           l l l
#             l

ALIGNMENT_FUNCS = {
    'a': [
        # a a a a a type of alignment
        [(0, 0), (1, 0), (2, 0), (3, 0), (4, 0)],
        # a
        # a
        # a
        # a
        # a
        # type of alignment
        [(0, 0), (0, 1), (0, 2), (0, 3), (0, 4)],
    ],
    'b': [
        # b b b b
        #       b
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (3, 0), (3, -1)],
        #   b
        #   b
        #   b
        # b b
        # type of alignment
        [(0, 0), (0, -1), (0, -2), (0, -3), (-1, -3)],
        # b
        # b b b b
        # type of alignment
        [(0, 0), (0, -1), (1, -1), (2, -1), (3, -1)],
        # b b
        # b
        # b
        # b
        # type of alignment
        [(0, 0), (1, 0), (0, -1), (0, -2), (0, -3)],
        # b b b b
        # b
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (3, 0), (0, -1)],
        # b b
        #   b
        #   b
        #   b
        # type of alignment
        [(0, 0), (1, 0), (1, -1), (1, -2), (1, -3)],
        #       b
        # b b b b
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (3, 0), (3, 1)],
        # b
        # b
        # b
        # b b
        # type of alignment
        [(0, 0), (1, 0), (0, 1), (0, 2), (0, 3)],
    ],
    'c': None,
    'd': None,
    'e': [
        # e
        # e
        # e e e
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (0, 1), (0, 2)],
        # e e e
        # e
        # e
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (0, -1), (0, -2)],
        #     e
        #     e
        # e e e
        # type of alignment
        [(0, 0), (0, 1), (0, 2), (-1, 0), (-2, 0)],
        # e e e
        #     e
        #     e
        # type of alignment
        [(0, 0), (-1, 0), (-2, 0), (0, -1), (0, -2)],
    ],
    'f': None,
    'g': None,
    'h': None,
    'i': None,
    'j': None,
    'k': [
        # k k
        #   k k k
        # type of alignment
        [(0, 0), (1, 0), (1, -1), (2, -1), (3, -1)],
        #   k
        # k k
        # k
        # k
        # type of alignment
        [(0, 0), (0, 1), (0, 2), (1, 2), (1, 3)],
        # k k k
        #     k k
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (2, -1), (3, -1)],
        #   k
        #   k
        # k k
        # k
        # type of alignment
        [(0, 0), (0, 1), (1, 1), (1, 2), (1, 3)],
        #     k k
        # k k k
        # type of alignment
        [(0, 0), (1, 0), (2, 0), (2, 1), (3, 1)],
        # k
        # k
        # k k
        #   k
        # type of alignment
        [(0, 0), (0, 1), (0, 2), (1, 0), (1, -1)],
        #   k k k
        # k k
        # type of alignment
        [(0, 0), (1, 0), (1, 1), (2, 1), (3, 1)],
        # k
        # k k
        #   k
        #   k
        # type of alignment
        [(0, 0), (0, 1), (1, 0), (1, -1), (1, -2)],
    ],
    'l': None,
    }

def mk_align_funcs(alignments):
    for 
    
class Puzzle(object):
    def __init__(self, dims):
        self._width, self._length = dims
        self._grid = []
        for i in range(self._width):
            self._grid.append(list(' '*self._length))
        self._solution = {}

    def __repr__(self):
        ret = ['-' * (self._length + 2)]
        for row in self._grid:
            ret.append('-' + (''.join(row)) + '-')
        ret.append('-' * (self._length + 2))
        return '\n'.join(ret)

    def is_available(self, pos, align_funcs):
        pivot_pos_func, all_ps_func = align_funcs
        pivot = pivot_pos_func(pos)
        points = all_ps_func(pivot)
        
        return all(self._grid[x][y] == ' ' for x, y in points)
    
        
        
'''
def solve_game(game, pieces):
    for p in pieces:
        for p_alignment in get_aligements(p):
            pos = next_available_position(game)
            if fit_piece(game, pos, p, p_alignment):      
'''

if __name__ == "__main__":
    pieces = list('abcdefghijkl') # all pieces

    game = Puzzle((5, 3))
    print(game)

    #solve_game(game, pieces)
    
