# python implementation of histogram exercise

def print_histogram(ns):
    counts, height = [0]*10, 0
    for n in ns:
        counts[n] += 1
        if counts[n] > height:
            height = counts[n]

    def make_line(cs):
        line = ''.join(map(lambda x : '*' if x > 0 else ' ', cs))
        for i, c in enumerate(cs):
            if c > 0: cs[i] -= 1
        return line

    lines = []
    for h in range(height):
        lines.append(make_line(counts))

    for l in reversed(lines):
        print(l)
    print("==========")
    print("0123456789")
        
    
if __name__ == "__main__":
    print_histogram([1, 4, 5, 4, 6, 6, 3, 4, 2, 4, 9])
