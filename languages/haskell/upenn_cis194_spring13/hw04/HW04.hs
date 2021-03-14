import qualified Data.List as DL
--exercise wholemeal programming
--use wholemeal programming practices breaking each function into a 
--pipeline of incremental transformations to an entire data structure

fun1 :: [Integer] -> Integer
fun1 [] = 1
fun1 (x:xs)
    | even x = (x-2) * fun1 xs
    | otherwise = fun1 xs

--calculating the product all even numbers adjusted by -2

fun1' = product . map (2 `subtract`) . filter even

fun2 :: Integer -> Integer
fun2 1 = 0
fun2 n | even n = n + fun2 (n `div` 2)
       | otherwise = fun2 (3*n+1)


-- f(10) = 10 + f(5) = 10 + f(16) = 10 + 16 + f(8) 
--       = 10 + 16 + 8 + f(4) = 10 + 16 + 8 + 4 + f(2)
--       = 10 + 16 + 8 + 4 + 2 + f(1)
--       = 40
--
-- it is equivalent to running the following f function on the starting 
-- data point, skip every odd number it encounter until it is 1
-- sum up all the even number it has seen so far

f n | even n = n `div` 2
    | otherwise = 3*n+1

fun2' = sum . filter even . takeWhile (>1) . iterate f 

--exercise folding with trees
--height of a binary tree is the length of a path from the root to the 
--deepest node. 
--single node tree, height is 0
--3 nodes, whose root has 2 children, height is 1
--balanced tree := |h(left)-h(right)| <= 1, recursively for left and right

data Tree a = Leaf | Node Integer (Tree a) a (Tree a) deriving (Show, Eq)

-- generates a balanced binary tree
foldTree :: [a] -> Tree a
foldTree = foldr insert Leaf 
    where 
        height Leaf = -1
        height (Node h _ _ _) = h

        insert x Leaf = Node 0 Leaf x Leaf
        insert x (Node h l n r) 
            | hl <= hr = let l' = insert x l
                             h' = max (height l') hr
                         in Node (h'+1) l' n r
            | otherwise = let r' = insert x r
                              h' = max (height r') hl
                          in Node (h'+1) l n r'
            where 
                hl = height l
                hr = height r

-- exercise 3 more folds
-- 1, implement a function 
-- xor :: [Bool] -> [Bool]
-- which returns True if and only if there are an odd number of Tree
-- values, regardless how many False in it

xor :: [Bool] -> Bool
xor = foldr run False 
    where 
        run b acc | not b = acc
                  | otherwise = not acc

--2, implement map as a fold
map' :: (a -> b) -> [a] -> [b]
map' f = foldr run [] 
    where 
        run a bs = (f a):bs

--3, implement foldl using foldr
-- foldr f z [x1, x2, ... xn] = x1 `f` (x2 `f` .. (xn `f` z).. )
-- foldl f z [x1, x2, ... xn] = (..((z `f` x1) `f` x2) `f` ...) `f` xn
--
-- let ff = flip f
-- foldr ==> (x2 `f` .. (xn `f` z)) `ff` x1
--       ==> (...((z `ff` xn) `ff`.. ) `ff` x2 ) `ff` x1
--
myFoldl :: (a -> b -> a) -> a -> [b] -> a
myFoldl f base xs = foldr (flip f) base $ reverse xs

--exercise 4 finding primes. Sieve of Sundaram
--given an integer n your function should generate all the odd prime
--numbers up to 2n+2

sieveOfSundaram n = filter (<=n) $ [i+j+2*i*j | j <- [1..n], i <- [1..j]]

--generates primes up to 2n+2
primes n = 2:(map (\x -> 2*x+1) $ [1..n] DL.\\ (sieveOfSundaram n))

main = do
     print $ fun1 [10..100]
     print $ fun1' [10..100]
     print $ map fun2 [1..20]
     print $ take 20 $ iterate f 10
     print $ map fun2' [1..20]
     print $ foldTree "ABCDEFGHIJ"
     print $ xor [False, True, False]
     print $ xor [False, True, False, False, True]
     print $ map' fun2 [1..20]
     print $ sieveOfSundaram 100
     print $ primes 100
