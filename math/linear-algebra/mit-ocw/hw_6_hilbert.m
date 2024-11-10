for i = 1:10
    fprintf("Hilbert of %d: \n", i)
    H = hilb(i)
    [L, U, P] = lu(H) % generate very small numbers
    d = det(H)
end
