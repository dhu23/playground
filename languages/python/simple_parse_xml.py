class Parser(object):
    def __init__(self, input_string):
        self.s = input_string
        self.i = 0

def satisfy(fn):
    pass

def parse_whitespace(st):
    i, s = st
    while i < len(s) and s[i] in (' \t\n'):
        i += 1
    return i, s

def parse_char(st):
    pass

def parse_num(st):
    pass

def parse_var(st):
    pass

def parse_tag(st):
    i, s = parse_whitespace(st)
    if i > len(s): return
        assert s[i] == '<'
        i += 1

if __name__ == "__main__":
    pass
