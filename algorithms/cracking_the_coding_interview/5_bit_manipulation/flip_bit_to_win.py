# using Kadena's algorithm

import itertools

def flip_and_win(x):
    print(bin(x), len(bin(x)[2:]))
    groups = []
    for k, seq in itertools.groupby(bin(x)[2:]):
        groups.append((k, len(list(seq))))

    print(groups)
    max_seq, curr_max_seq = 0, 0
    for i, (k, count) in enumerate(groups):
        if k == '0' and count == 1:
            curr_max_seq = count # 1
            if i > 0:
                curr_max_seq += groups[i-1][1]
            if i < len(groups)-1:
                curr_max_seq += groups[i+1][1]
        
        elif k == '0' and count > 1:
            # easier go with group of 1s at (i-1) or (i+1)
            curr_max_seq = max(1 + groups[i-1][1] if i > 0 else 0,
                               1 + groups[i+1][1] if i < len(groups)-1 else 0)
        
        max_seq = max(max_seq, curr_max_seq)    
                
    return max_seq

if __name__ == "__main__":
    print(flip_and_win(0xfffa053b))
    print(flip_and_win(0xfff00afc))
