fib:{$[x<2;x;fib[x-2]+fib[x-1]]};
timeTest:{sum fib each 1_til 40};

show timeTest[];
exit 0;
