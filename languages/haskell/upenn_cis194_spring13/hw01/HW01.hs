
--exercise 1. 
--we need to first find the digits of a number. Define the functions
--toDigits :: Integer -> [Integer]
--toDigitsRev :: Integer -> [Integer]
--for non-positive input, return []

toDigitsRev :: Integer -> [Integer]
toDigitsRev n 
    | n <= 0 = []
    | n < 10 = [n]
    | otherwise = q : (toDigitsRev p)
    where 
        (p, q) = n `divMod` 10

toDigits = reverse . toDigitsRev

--exercise 2.
--double every other number beginning from the right, second to last, 
--fourth to last...
doubleEveryOther :: [Integer] -> [Integer]
doubleEveryOther = reverse . zipWith ($) (cycle [id, (*2)]) . reverse

doubleEveryOther' [] = []
doubleEveryOther' ns@(n:rest)
    | even $ length ns = zipWith ($) (cycle [(*2), id]) ns
    | otherwise = n : (doubleEveryOther' rest)

--exercise 3. sum digits
sumDigits :: [Integer] -> Integer
sumDigits = sum . map (sum . toDigitsRev) 

--exercise 4. validate sumDigits. has to be a multiplier of 10
validate :: Integer -> Bool
validate n = (getVal n) `mod` 10 == 0
    where 
        getVal = sumDigits . doubleEveryOther . toDigits

testCreditCard = do
    print $ toDigits 1234
    print $ toDigitsRev 1234
    print $ doubleEveryOther' [8, 7, 6, 5]
    print $ doubleEveryOther' [1, 2, 3]
    print $ sumDigits [16, 7, 12, 5]
    print $ validate 4012888888881881
    print $ validate 4012888888881882

--exercise 5. The towers of Hanoi
type Peg = String
type Move = (Peg, Peg)
-- get all disks on peg 1 to peg 3
hanoi :: Integer -> Peg -> Peg -> Peg -> [Move]
hanoi n a b c
    | n <= 0 = []
    | n == 1 = [(a, c)]
    --to achieve hanoi n a b c, do hanoi (n-1) a c b 
    --then move hanoi 1 a b c, then do
    --hanoi (n-1) b a c
    | otherwise = (hanoi (n-1) a c b) ++
                  [(a, c)] ++
                  (hanoi (n-1) b a c)

--exercise 6. what if there are four pegs
hanoi' :: Integer -> Peg -> Peg -> Peg -> Peg -> [Move]
hanoi' n a b c d 
    | n <= 0 = []
    | n == 1 = [(a, d)]
    -- | n == 2 = [(a, b), (a, d), (b, d)]
    -- | n == 3 = [(a, b), (a, c), (a, d), (c, d), (b, d)]
    | otherwise = 
        case foldl step Nothing $ map move [1..n-1] of
            Nothing -> []
            (Just x) -> x
    where
        move k = (hanoi' (n-k) a c d b) ++
                 (hanoi k a c d) ++ 
                 (hanoi' (n-k) b a c d)
        step Nothing ms = Just ms
        step (Just bestSoFar) ms
            | length bestSoFar > length ms = Just ms
            | otherwise = Just bestSoFar


-- this one has memory issue
hanoi'' :: Integer -> Peg -> Peg -> Peg -> Peg -> [Move]
hanoi'' n a b c d
    | n <= 0 = []
    | n == 1 = [(a, d)]
    -- move one from a to b
    -- hanoi k a d c move k disks from a to c
    -- move one from b to c
    -- now a and c are taken, b and d are empty
    -- move n-k-1 from a to d with b as buffer
    -- move k+1 on c to d with a, b as buffer
    | otherwise = 
        case foldl step Nothing $ map move [1..n-1] of
            Nothing -> []
            (Just x) -> x
    where 
        move k = [(a, b)] ++ 
                 (hanoi k a b c) ++ 
                 [(b, c)] ++
                 (hanoi (n-k-1) a b d) ++
                 hanoi'' (k+1) c a b d
        step Nothing ms = Just ms
        step (Just bestSoFar) ms
            | length bestSoFar > length ms = Just ms
            | otherwise = Just bestSoFar

testHanoi = do
    print $ hanoi 2 "a" "b" "c"
    print $ hanoi 3 "a" "b" "c"
    print $ hanoi' 3 "a" "b" "c" "d"
    print $ hanoi' 4 "a" "b" "c" "d"
    print $ hanoi' 5 "a" "b" "c" "d"
    print $ hanoi' 15 "a" "b" "c" "d"
    --print $ hanoi'' 4 "a" "b" "c" "d"

main = do
    testCreditCard
    testHanoi
