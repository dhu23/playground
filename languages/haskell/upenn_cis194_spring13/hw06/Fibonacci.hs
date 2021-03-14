{-# LANGUAGE FlexibleInstances #-}
{-# OPTIONS_GHC -fno-warn-missing-methods #-}

--exercise 1 
--translate the definition of Fibonacci numbers directly into a recursive
--function defition of type 
fib :: Integer -> Integer
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

fibs1 = map fib [0..]


--exercise 2: define fibs2 :: [Integer] so that the first n elements of 
--fibs2 require only O(n) additions.
--fn = f(n-1) + f(n-2)
--f(n-1) = f(n-2) + f(n-3)
-- ...
--f2 = f1 + f0
--so f(n) = f(n-2) + f(n-3) + ... + f(0) + f(1)
--and f(n-1) =       f(n-3) + ... + f(0) + f(1)
--this is the mathematical reason that scanl works
--and every recursion grows the series by 2
fibs2 = scanl (+) 0 (1:fibs2)


--exercise 3. define streams like lists but with only a cons constructor

data Stream a = Cons a (Stream a)
streamToList :: Stream a -> [a]
streamToList (Cons x xs) = x : streamToList xs

instance Show a => Show (Stream a) where
    show = show . take 20 . streamToList 

--exercise 4. simple tools for working with streams
streamRepeat :: a -> Stream a
streamRepeat x = Cons x (streamRepeat x)

streamMap :: (a -> b) -> Stream a -> Stream b
streamMap f (Cons a as) = Cons (f a) (streamMap f as)

streamFromSeed :: (a -> a) -> a -> Stream a
streamFromSeed f a = Cons a (streamFromSeed f (f a))

--exercise 5
nats :: Stream Integer
nats = streamFromSeed succ 0

--ruler funciton 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4..
--  power of 2   1, 2, 1, 4, 1, 2, 1, 8, 1, 2, 1, 4, 1, 2, 1, 16..
--  index        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16..
--where the nth element in the stream (assuming the first one is n=1)
--is the largest power of 2 which evenly divides n

--oddly indexed ones will be 0, corresponding to power=1
--evenly indexed ones: every other element is a 2
--                     every 4th element is a 4
--                     every 8th element is a 8
--look at the evenly distributed ones
--they are 1, 2, 1, 3, 1, 2, 1, 4...
--growth: 1
--        1 (1+1) = 1 2
--        1 2 1 2+1 = 1, 2, 1, 3
--        1, 2, 1, 3, 1, 2, 1, 3+1 = 1, 2, 1, 3, 1, 2, 1, 4
--evengrow generates the part that is to be appended
--[1] ++ evengrow [1] ++ evengrow ([1] ++ evengrow [1]) 
-- ++ evengrow( [1] ++ evengrow  [1] ++ evengrow ([1] ++ evengrow [1]))
-- ...
--
--
-- found from internet:
-- another way of looking at it
-- the original list is the interleave of 0 and 1, 2, 1, 3, 1, 2, 1, 4...
-- which is the interleave of 1 and 2, 3, 2, 4, ..
-- which is the interleave of 2 and 3, 4, 3, 5,..


flatStream :: Stream [a] -> Stream a
flatStream (Cons [] streams) = flatStream streams
flatStream (Cons (x:xs) streams) = Cons x (flatStream (Cons xs streams))

evengrow :: [Integer] -> [Integer]
evengrow xs = zipWith ($) incrementLast xs
    where 
        len = length xs
        incrementLast = (replicate (len-1) id) ++ [(+1)]

evengrow' xs = xs ++ evengrow xs

evenStream = flatStream $ streamFromSeed evengrow [1]

oddStream = streamRepeat 0

interleaveStreams :: Stream a -> Stream a -> Stream a
interleaveStreams (Cons x xs) ys = Cons x (interleaveStreams ys xs)

interleaveStreams' (Cons x xs) (Cons y ys) =
    Cons x (Cons y (interleaveStreams' xs ys))

ruler' :: Integer -> Stream Integer
ruler' n = interleaveStreams (streamRepeat n) (ruler' (n+1))
-- expansion:
-- interleave (srepeat n) (interleave (srepeat (n+1) (interleave...)))
-- interleaveStream' definition will cause every interleave to deconstruct
-- interleaveStream definition however won't and will expand the list
-- one at a time

ruler :: Stream Integer
ruler = ruler' 0

--generating functions:
--a0+a1 x + ax x x + ... + an x x x x ..x 
--store a0, a1, a2 in a Stream Integer
--exercise 6

-- by noting that x = 0 + 1 x + 0 x^2 + ...
x :: Stream Integer
x = Cons 0 (Cons 1 (streamRepeat 0))

-- define an instance of Num type class for Stream Integer.
-- 1) you should implement the fromInteger function, note that
-- n = n + 0 x + 0 x^2 ...
-- 2) define negate to negate a generating function, negating all a's
-- 3) implement (+)
-- 4) multiplication as well. A = a0+xA', B=b0+xB'
-- AB = a0b0 + x(a0B'+A'B)=a0b0+x(a0B'+b0A')+x^2(A'B')
-- however the further expansion to x^2 wont work with the code
instance Num (Stream Integer) where
    fromInteger n = Cons 1 (streamRepeat 0)
    negate (Cons a as) = Cons (negate a) (negate as)
    (Cons a as) + (Cons b bs) = Cons (a+b) (as + bs)
    (Cons a as) * bstream@(Cons b bs) = Cons (a * b) rest
        where 
            rest = (streamMap (*a) bs) + as * bstream
    --(Cons a as) * (Cons b bs) = 
    --Cons (a*b) (Cons ((streamMap (*a) bs) + (streamMap (*b) as)) (as*bs))

-- penultimate step is to implement instance of Fractional for Stream
-- Q = A/B = (a0+xA')/(b0+xB')
-- Q = (a0/b0) + x(1/b0)(A'-QB')

instance Fractional (Stream Integer) where
    (Cons a as) / (Cons b bs) = q
        where 
            q = Cons (a `div` b) $ streamMap (`div` b) (as-bs*q)


--F(x) = x/(1-x-x*x)
fibs3 :: Stream Integer
fibs3 = x / (1 - x - x^2)

--Fibonacci number via matrices
--it is possible to compute the nth Fibonacci number with only 
--O(log n) arithmetic operations. 
--consider 2 by 2 matrix defined by F = [1 1; 1 0] 
--F^2 = [2 1; 1 1]
--F^3 = [3 2; 2 1]
--F^4 = [5 3; 3 2]
--F^5 = [8 5; 5 3]
--F^n = [F(n+1) F(n); F(n) F(n-1)]
--exponentiation can be implemented in logirthmic time using binary
--exponetiation algorithm. 
--x^n = (x^(n/2))^2 for even n; x (x^((n-1)/2)^2) for odd n
--
--the punchline is that Haskell's exponentiation operator (^) already
--uses this algorithm. 
--exercise 7

data Matrix a = M a a a a deriving (Show)

instance Num (Matrix Integer) where
    (M a1 b1 c1 d1) * (M a2 b2 c2 d2) = 
        M (a1*a2+b1*c2) (c1*a2+d1*c2) (a1*b2+b1*d2) (c1*b2+d1*d2)

fib4 :: Integer -> Integer
fib4 n
    | n == 0 = 0
    | otherwise = getC $ (M 1 1 1 0)^n
        where 
            getC (M _ _ c _) = c

main = print 5
