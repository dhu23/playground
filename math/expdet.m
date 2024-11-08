function avg = expdet(type, n, repeat)
    if type == 0
        % rand
        v = 0;
        for r = 1:repeat
            v = v + det(rand(n));
        end
        avg = v / n;
    else
        % randn
        v = 0;
        for r = 1:repeat
            v = v + det(randn(n));
        end
        avg = v / n;
    end
end
