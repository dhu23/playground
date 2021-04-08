def within_board(x, y):
    return 0 <= x < 8 and 0 <= y < 8

def _chances_within(x, y, m, cache):
    if (x, y, m) in cache:
        return cache[(x, y, m)]

    if m == 0:
        ret = within_board(x, y)
        cache[(x, y, m)] = 1 if ret else 0
        return ret

    ret = 0.125 * sum([
        _chances_within(x+2, y+1, m-1, cache),
        _chances_within(x+2, y-1, m-1, cache),
        _chances_within(x-2, y+1, m-1, cache),
        _chances_within(x-2, y-1, m-1, cache),
        _chances_within(x+1, y+2, m-1, cache),
        _chances_within(x+1, y-2, m-1, cache),
        _chances_within(x-1, y+2, m-1, cache),
        _chances_within(x-1, y-2, m-1, cache),
        ])
    cache[(x, y, m)] = ret
    return ret

def chances_within(x, y, m):
    cache = {}
    return _chances_within(x, y, m, cache)
