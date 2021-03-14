stocks:4 5 10 11 4 5 9 4;


profit:{[x]
    dx:1_deltas x;
    sum dx[where dx>0]
    };


profit2: {[p]
    gp:group maxs rp:reverse p;
    sum key[gp]-min each rp gp
    };
