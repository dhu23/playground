K = toeplitz([2 -1 0 0 0 0 0 0 0])

% or
% K = 2*eye(2) + diag(-1*ones(1, 8), 1) + diag(-1*ones(1, 8), -1)

b = 10*ones(9, 1)


x = K \ b

plot([1:9], x)


% another way to look at solution is
rref([K, b])
