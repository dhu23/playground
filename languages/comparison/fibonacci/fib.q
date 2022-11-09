fib:{$[x<2;x;fib[x-2]+fib[x-1]]};
timeTest:{[v]sum fib each 1_til v};

show timeTest[40];
exit 0;
