% pretend a linear relationship between x and y as y = 2x+10
% An experiment is carried out to measure the coefficent and constant


dataX = [-5:0.5:5]';

N = size(dataX)(1, 1) % 21

dataY = 2*dataX + 10 + 3*randn(N)(:, 1);


% we are solve y = ax + b here
% [x, 1] a = y where a is [a, b]'

A = [dataX, ones(21, 1)];
b = dataY;

a = inv(A'*A) * (A'*b)

e = A*a - b
summary = [A*a, b, e]

