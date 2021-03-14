def rebase(i, base_seq):
    base = len(base_seq)
    assert len(set(base_seq)) == base

    x = i
    ret = []
    while x != 0:
        x, r = divmod(x, base)
        ret.append(base_seq[r])
    ret.reverse()
    return ''.join(ret)


def hornor(s, base_seq):
    base = len(base_seq)
    assert len(set(base_seq)) == base

    x = 0
    for c in s:
        i = base_seq.find(c)
        if i == -1:
            raise Exception('not in base seq:%s' % c)
        x = x*base + i

    return x
