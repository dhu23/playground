--implement the function log2, which computes the integer log (base 2)
--of its argument

log2 :: Integer -> Integer
log2 n
    | n == 1 = 0
    | n == 2 = 1
    | otherwise = 1 + log2 (n `div` 2)


replicate' :: Int -> a -> [a]
replicate' n x 
    | n <= 0 = []
    | otherwise = x : replicate (n-1) x

(!!>) :: [a] -> Int -> a
[] !!> _ = error "list is empty"
(x:xs) !!> k 
    | k < 0 = error "index has to be positive"
    | k == 0 = x
    | otherwise = xs !!> (k-1)

zip' :: [a] -> [b] -> [(a, b)]
zip' [] _ = []
zip' _ [] = []
zip' (x:xs) (y:ys) = (x, y) : zip' xs ys

length' :: [a] -> Int
length' xs = run xs 0
    where
    run [] k = k
    run (x:xs) k = 1 + run xs k
