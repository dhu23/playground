split:"," vs;
splits:{{`$x} each split x};

join:"," sv;
joins:{join string x};
joinq:{"'", ("', '" sv string x), "'"};
