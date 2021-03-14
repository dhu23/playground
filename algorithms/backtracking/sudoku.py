import itertools

# anatomy of sudoku board
#  idx 0   idx 1   idx 2
#        |       |
#  one   |  two  | three     idx 0
#        |       |
# ------------------------
#        |       |
#  four  | five  |  six      idx 2
#        |       |
# ------------------------
#        |       |
#  seven | eight |  nine     idx 3
#        |       |
#


class Sudoku(object):
    NUMBERS = set(range(1, 10, 1)) # 1, 2, 3, 4, 5, 6, 7, 8, 9
    '''
    class Box(object):

        BOX_NAMES = {
            (0, 0): 'Up Left Box',
            (0, 1): 'Up Center Box',
            (0, 2): 'Up Right Box',
            (1, 0): 'Center Left Box',
            (1, 1): 'Center Box',
            (1, 2): 'Center Right Box',
            (2, 0): 'Down Left Box',
            (2, 1): 'Down Center Box',
            (2, 2): 'Down Right Box'}
        
        def __init__(self, row, col):
            self.box_row = row
            self.box_col = col
            self.box_id = self.box_row*3+self.box_col

        def __repr__(self):
            return Sudoku.Box.BOX_NAMES[(self.box_row, self.box_col)]
    
    BOXES = dict(
        (box_pos, Box(*box_pos)) for box_pos in itertools.product(
            [0, 1, 2], [0, 1, 2]))
    
    ''' 
    class Position(object):
        def __init__(self, **kwargs):
            if 'row' in kwargs and 'col' in kwargs:
                self.pos = Sudoku.Position.get_pos(
                    kwargs['row'], kwargs['col'])
                self.row, self.col = kwargs['row'], kwargs['col']
            elif 'pos' in kwargs:
                self.row, self.col = Sudoku.Position.get_loc(
                    kwargs['pos'])
                self.pos = kwargs['pos']
            else:
                raise RuntimeError("Bad position input=%s" % kwargs)
            
            #self.box = Sudoku.BOXES[(self.row/3, self.col/3)]
            self.box = (self.row/3, self.col/3)
            #print 'pos=%d,row=%d,col=%d,box_row=%d,box_col=%d,box_id=%d' % (
            #    self.pos, self.row, self.col,
            #    self.box_row, self.box_col, self.box_id)

        def __repr__(self):
            return str((self.row, self.col))
            
        @staticmethod
        def get_pos(row, col):
            assert 0 <= row < 9
            assert 0 <= col < 9
            return row*9+col

        @staticmethod
        def get_loc(pos):
            assert 0 <= pos < 81
            return divmod(pos, 9)

        
    def __init__(self, numbers):
        if len(numbers) != 81:
            raise RuntimeError("number size is not 81")

        def check_number(x):
            if x is None: return True
            if not isinstance(x, int): return False
            if x > 9 or x < 1: return False
            return True
        
        if any((not check_number(n) for n in numbers)):
            raise RuntimeError("invalid number input")
        
        self.numbers = numbers
        
        self.board, self.pos_to_fill = [], []
        self.box_to_pos, self.row_to_pos, self.col_to_pos = {}, {}, {}

        def add_to_group_to_pos_map(m, pos, group):
            if group not in m: m[group] = [pos]
            else: m[group].append(pos)
                
        for pos in range(81):
            position = Sudoku.Position(pos=pos)
            self.board.append(position)
            if self.numbers[pos] is None:
                self.pos_to_fill.append(pos)
        
            add_to_group_to_pos_map(self.box_to_pos, pos, position.box)
            add_to_group_to_pos_map(self.row_to_pos, pos, position.row)
            add_to_group_to_pos_map(self.col_to_pos, pos, position.col)
            
        self.possibles = [set(range(1, 10, 1)) for pos in range(81)]
        for pos in range(81):
            num = self.numbers[pos]
            if num is None: 
                self.possibles[pos] -= self._get_seen(pos)
            else:
                self.possibles[pos] = set((num,))
        
            
    def fill(self, pos, fill):
        # print 'pos=', pos
        self.numbers[pos] = fill

    def _get_seen(self, pos):
        pos_to_check = []
        position = self.board[pos]
        pos_to_check.extend(pos for pos in self.row_to_pos[position.row])
        pos_to_check.extend(pos for pos in self.col_to_pos[position.col])
        pos_to_check.extend(pos for pos in self.box_to_pos[position.box])
        pos_to_check.remove(pos)

        return set([self.numbers[pos] for pos in pos_to_check
                    if self.numbers[pos] is not None])
        
    def verify_fill(self, pos, fill):
        return False if fill in self._get_seen(pos) else True
        

    def display(self):
        display = []
        for i in range(81):
            board_position = self.board[i]
            num = self.numbers[i]

            display.append(str(num) if num is not None else ' ')

            if board_position.col in (2, 5):
                display.append('|')

            if board_position.col == 8:
                display.append('\n')
                if board_position.row in (2, 5):
                    display.extend(['-']*11)
                    display.append('\n')

        print ''.join(display)
            
count = 0
        
def solve(sdk, pos_to_fill, index, fill):
    assert 0 <= index < len(pos_to_fill)
    global count
    count += 1
    #if index < 10:
        
    sdk.display()

    #if index > 10:
    #    raise RuntimeError("intended")
        
    print 'index=%d,fill=%d' % (index, fill)
    pos = pos_to_fill[index][0]
    while fill not in sdk.possibles[pos] or not sdk.verify_fill(pos, fill):
        if fill > 9: break
        fill += 1
    
    print 'fill=%d' % fill
    if fill <= 9: # found a good fill
        pos_to_fill[index][1] = fill
        sdk.fill(pos_to_fill[index][0], fill)
        if index == len(pos_to_fill)-1:
            sdk.display()
            return True
        else:
            ret = solve(sdk, pos_to_fill, index+1, 1)
            if ret is True:
                return ret
            else:
                pos_to_fill[index][1] = None
                sdk.fill(pos_to_fill[index][0], None)
                if fill == 9:
                    return False
                else:
                    return solve(sdk, pos_to_fill, index, fill+1)
                
    else:
        # fill is a bad try. back tracking
        if index == 0:
            raise RuntimeError("Cannot solve")
        else:
            pos_to_fill[index][1] = None
            sdk.fill(pos_to_fill[index][0], None)
            return False
            #return solve(sdk, pos_to_fill, index-1, pos_to_fill[index-1][1]+1)
        
        
        
def solve_sudoku(sdk):
    pos_to_fill = [[pos, None] for pos in sdk.pos_to_fill]
    print len(pos_to_fill)
    solve(sdk, pos_to_fill, 0, 1)
    print count
    
    

if __name__ == "__main__":


    sdk = Sudoku([
        None, None, 6, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, None,
        None, 9, None, None, None, 7, None, 4, None,
        1, None, 4, None, 8, None, None, 3, None,
        None, None, None, None, 6, None, None, None, None,
        None, 5, None, 7, 4, None, None, None, 9,
        2, 1, None, 8, 9, 4, 7, None, None,
        None, 3, None, 6, None, None, 9, None, 5,
        None, None, 7, None, 3, 1, None, None, 4
        ])

    sdk.display()

    solve_sudoku(sdk)

    sdk_flash = Sudoku([
        2, 8, 4, None, None, 5, 9, 7, None,
        5, 7, None, None, None, None, None, 1, None,
        1, 6, None, 7, 4, 2, 8, None, 5,
        None, 3, None, None, None, 9, 4, 6, 2,
        None, None, 2, 1, 6, None, None, None, 7,
        None, 9, 5, 4, None, None, 1, None, 3,
        3, 1, 8, None, None, None, 6, None, 9,
        4, 2, None, None, 3, None, None, 5, 8,
        9, None, 7, 2, 8, 6, 3, 4, 1])

    sdk_flash.display()
    solve_sudoku(sdk_flash)
