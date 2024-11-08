function [Q, R] = myqr(A)
    [m, n] = size(A);

    Q = A;
    R = eye(n);

    for j = 1:n
        % fprintf("----------------- doing j=%d\n", j)
        % select previous columns, and project Q(:, j) onto them.
        % calculate the residual as the new Q(:, j)
        col = Q(:, j);

        % say A, B are orthonormal, while c is to be normalized
        % c2 = c - (A'c)*A - (B'c)*B
        % C = c2 / norm(c2)
        % so c = (A'c)*A + (B'c)*B + norm(c2) * C, 
        % or A'c, B'c, norm(c2) are the column vector elements of R

        for prev = 1:j-1
            ortho = Q(:, prev);
            coeff = ortho'*col;
            R(prev, j) = coeff;
            col = col - coeff*ortho;
        end

        jnorm = norm(col);
        Q(:, j) = col / jnorm;
        R(j, j) = jnorm;
        % test = Q*R
    end
end
