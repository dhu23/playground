A = rand(2, 4)

% A's null space
N = null(A) 

% B column is perpendicular to every column in N, so B is vertical to N(A)
% B column is the row space of A
B = null(N')


% basically A and B' span the same row space
rref(A)
rref(B')
