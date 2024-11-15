% constructing a random matrix with +/- 1 determinant
% d is the determinant of D, either 1 or -1
function [D, d] = det1(n, row)
    D = eye(n)
    d = 1
    for i = 1:randi([1 n])*15
        indices = randperm(n);
        
        if row
            rowop = true;
            op = "row";
        else
            if rand < 0.5
                rowop = true;
                op = "row";
            else
                rowop = false;
                op = "column";
            end
        end
        
        if rand < 0.25
            
            % switch two rows or columns
            %fprintf("switching %s: %d <=> %d\n", op, indices(1), indices(2));
            d *= -1;
            if rowop
                D([indices(2), indices(1)], :) = D(indices(1:2), :);
            else
                D(:, [indices(2), indices(1)]) = D(:, indices(1:2));
            end
            %D
        else
            % add one row to another
            alpha = randi([-3, 3]);
            if alpha ~= 0
                %fprintf("adding %d x %s %d to %s %d, \n", alpha, op, indices(1), op, indices(2));
                if rowop
                    D(indices(2), :) += alpha * D(indices(1), :);
                else
                    D(:, indices(2)) += alpha * D(:, indices(1));
                end
                %D
            end
        end
    end
end
