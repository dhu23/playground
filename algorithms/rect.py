def find_rect(points): # a list of tuples
    _pset = set(points)
    
    ret = []
    for x1, y1 in points:
        for x2, y2 in points:
            if x1 < x2 and y1 < y2:
                # fixing bottom left and top right
                # tl(x1, y2)      tr(x2, y2) 
                # bl(x1, y1)      br(x2, y1)
                if (x1, y2) in _pset and (x2, y1) in _pset:
                    ret.append(((x1, y1), (x2, y2)))

    return ret
