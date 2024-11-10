function [L, U, E] = mylu(A)
    [m, n] = size(A);
    if m ~= n
        error('mylu:InputNotSquare', 'expecting square matrix');
    end

    le = [A, eye(m)];
    U = eye(m);

    for i = 1:m-1 % clear all elements below A(i, i)
        % pivot is i
        for j = (i+1):m
            e = le(j, i) / le(i, i);
            U(j, i) = e;
            le(j, :) = le(j, :) - le(i, :) * e;
        end
    end

    L = le(:, 1:m);
    E = le(:, m+1:2*m);
end
