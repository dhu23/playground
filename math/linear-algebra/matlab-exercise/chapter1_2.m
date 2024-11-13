n = 200;
A = floor(10*rand(n)); b = sum(A')'; z = ones(n, 1);
% z is the solution to Ax = b


tic, x = A\b; toc
tic, y = inv(A)*b; toc

max(abs(x-z))
max(abs(y-z))
