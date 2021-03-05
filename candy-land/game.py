from collections import namedtuple
from collections import UserList
from random import shuffle
from functools import singledispatch

StartTile = namedtuple('StartTile', ['dummy'])
RegularTile = namedtuple('RegularTile', ['color'])
PassStartTile = namedtuple('PassStartTile', ['color', 'name'])
PassEndTile = namedtuple('PassEndTile', ['color', 'name'])
LicoriceTile = namedtuple('LicoriceTile', ['color'])
SpecialTile = namedtuple('SpecialTile', ['name'])
RainbowTile = namedtuple('RainbowTile', ['name'])


RegularCard = namedtuple('RegularCard', ['color', 'count'])
SpecialCard = namedtuple('SpecialCard', ['name'])


class GameError(Exception):
    pass


class TerminateGame(Exception):
    def __init__(self, winner):
        self.winner = winner


class Player(object):
    def __init__(self, color):
        self.color = color
        self.wait = 0
        self.cards = []
        self.pos = 0

    def __repr__(self):
        return '%s player(wait=%d, pos=%d)' % (self.color, self.wait, self.pos)


@singledispatch
def _is_special_tile(x, name):
    return False

@_is_special_tile.register(SpecialTile)
def _(x, name):
    return x.name == name


@singledispatch
def _is_the_last_tile(x):
    return False

@_is_the_last_tile.register(RainbowTile)
def _(x):
    return True


@singledispatch
def _has_color(x, color):
    return x.color == color

@_has_color.register(StartTile)
def _(x, color):
    return False

@_has_color.register(SpecialTile)
def _(x, color):
    return False

@_has_color.register(RainbowTile)
def _(x, color):
    return True


@singledispatch
def _is_pass_end(x, name):
    return False


@_is_pass_end.register(PassEndTile)
def _(x, name):
    return x.name == name


@singledispatch
def _adjust_by_tile(x, player, tiles):
    pass


@_adjust_by_tile.register(RainbowTile)
def _(x, player, tiles):
    raise TerminateGame(player.color)


@_adjust_by_tile.register(PassStartTile)
def _(x, player, tiles):
    curr_pos = player.pos+1
    while True:
        t = tiles[curr_pos]
        if _is_pass_end(t, x.name):
            player.pos = curr_pos
            break
        curr_pos += 1
    print('player %s goes through %s to pos=%s' % (
        player.color, x.name, curr_pos))


@_adjust_by_tile.register(LicoriceTile)
def _(x, player, tiles):
    player.wait = 1
    print('player %s waits on LicoriceTile' % player.color)


@singledispatch
def _move_by_card(x, player, tiles): # assuming x is SpecialCard

    def _find_special_pos(special_name):
        for i, tile in enumerate(tiles):
            if _is_special_tile(tile, special_name):
                return i
        raise GameError('cannot find card with name=%s' % special_name)

    player.pos = _find_special_pos(x.name)
    print('player %s lands on special card %s' % (player.color, x.name))
    player.cards.append(x)


@_move_by_card.register(RegularCard)
def _(x, player, tiles):

    def _next_tile_with_color(c, pos):
        curr_pos = pos+1
        while True:
            t = tiles[curr_pos]
            if _is_the_last_tile(t):
                raise TerminateGame(player.color)

            if _has_color(t, c):
                return curr_pos
            curr_pos += 1
            
    clr, cnt = x
    current_pos = player.pos
    while cnt > 0:
        current_pos = _next_tile_with_color(clr, current_pos)
        cnt -= 1
    player.pos = current_pos
    print('player %s lands on pos=%d' % (player.color, player.pos))
    _adjust_by_tile(tiles[current_pos], player, tiles)


class CandyLand(object):

    def __init__(self, *args):

        self.players = []
        for player_color in args:
            if player_color not in ('Blue', 'Green', 'Red', 'Yellow'):
                raise RuntimeError('Unknown player color %s' % player_color)
            self.players.append(Player(color=player_color))

        self.current_player_id = 0

        self.tiles = [
            StartTile(dummy=None),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            PassStartTile(color='Blue', name='peppermint'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            SpecialTile(name='Cupcake'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            SpecialTile(name='Icecream'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            PassStartTile(color='Yellow', name='gummybear'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            PassEndTile(color='Yellow', name='gummybear'),
            SpecialTile(name='Gummystar'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            LicoriceTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            PassEndTile(color='Yellow', name='peppermint'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            SpecialTile(name='Gingerbreadman'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            LicoriceTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            SpecialTile(name='Lollipop'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            SpecialTile(name='Popsicle'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            SpecialTile(name='Chocolate'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RegularTile(color='Red'),
            RegularTile(color='Purple'),
            RegularTile(color='Yellow'),
            RegularTile(color='Blue'),
            RegularTile(color='Orange'),
            RegularTile(color='Green'),
            RainbowTile(name='final'),
        ]

        self.deck = []
        for clr in ['Red', 'Purple', 'Yellow', 'Blue', 'Orange', 'Green']:
            self.deck.extend([RegularCard(color=clr, count=1)]*6)
            if clr in ['Red', 'Purple', 'Yellow', 'Blue']:
                double_num = 4
            else:
                double_num = 3
            self.deck.extend([RegularCard(color=clr, count=2)]*double_num)
        self.deck.extend([
            SpecialCard(name='Cupcake'),
            SpecialCard(name='Icecream'),
            SpecialCard(name='Gummystar'),
            SpecialCard(name='Gingerbreadman'),
            SpecialCard(name='Lollipop'),
            SpecialCard(name='Popsicle'),
            SpecialCard(name='Chocolate'),
        ])
        self._shuffle()
        
    def _shuffle(self):
        shuffle(self.deck)

    def take_turn(self):
        
        def _next(player_id):
            return (player_id+1) % len(self.players)

        next_player_id = _next(self.current_player_id)
        if self.players[next_player_id].wait > 0:
            self.players[next_player_id].wait -= 1
            next_player_id = _next(next_player_id)
        self.current_player_id = next_player_id
        print('player=%d to take the turn' % self.current_player_id)
        return self.current_player_id

    def move(self):
        if not self.deck:
            raise GameError('no more cards to draw')

        player = self.players[self.current_player_id]

        # convention: always pop cards from the back of the deck
        card = self.deck.pop()
        print('draws card=%s' % repr(card))

        player.cards.append(card)
        _move_by_card(card, player, self.tiles)
        print('player state=%s' % repr(player))

        self.take_turn()

    def play(self):
        try:
            while True:
                self.move()
        except TerminateGame as win:
            print('we got a winner:%s' % win)


    
if __name__ == '__main__': 
    cl = CandyLand('Red', 'Blue')
    cl.play()
