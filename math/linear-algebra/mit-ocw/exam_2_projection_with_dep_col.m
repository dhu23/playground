A = [1 0 0; 1 1 1; 1 3 9; 1 4 16];
b = [0 8 8 20]';


xhat = inv(A'*A)*A'*b
% uniquely projected vector p that's closest to b
p = A*xhat


% if A contains dependent columns, 
depcol = A(:, 1) + A(:, 2);
A1 = [A depcol]

% technically, we take 1 off xhat(1) and xhat(2), and give to xhat(4)
xhat0 = [xhat; 0] + [-1; -1; 0; 1];
p0 = A1*xhat0

% if we use the formula, we need to solve (A1'*A1)*xhat1 = A1'*b
S1 = A1'*[A1, b]
rrefS1 = rref(S1)
S = A'*[A, b]
rrefS = rref(S)

% S1 has infinite many solutions, it's nullspace is
xs = [-1; -1; 0; 1];
% its particular solution is 
xp = [xhat; 0];
% so its solution is in 
solutionSpace = [xp xs]

solutionSpace * [1; 1] == xhat0

xhat1 = solutionSpace * [1; 0]
p1 = A1*xhat1

xhat2 = solutionSpace * [1; -1]
p2 = A1*xhat2

xhat3 = solutionSpace * [1; 2]
p3 = A1*xhat3

p1 == p0
p2 == p0
p3 == p0