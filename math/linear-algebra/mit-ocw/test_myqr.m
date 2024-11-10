A = eye(4) - diag([1, 1, 1], -1)


[Q, R] = myqr(A)

A2 = Q*R

[Q0, R0] = qr(A)

A0 = Q0*R0
