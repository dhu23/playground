import qualified Data.List as DL
import qualified Data.Map as DM
import qualified Data.Set as DS
import qualified Data.Char as DC
import qualified Control.Monad as CM

--problem 1 multiples of 3 and 5
--if we list all the natural numbers below 10 that are multiples of 3 and 5
--we get 3, 5, 6, 9. The sum of these multiples is 23.
--
--Find the sum of all multiples of 3 or 5 below 1000

problem1 :: Int -> Int
problem1 limit = sum $ filter isMultiple [1..limit-1]
    where isMultiple n = n `mod` 3 == 0 || n `mod` 5 == 0


--problem 2 Even Fibonacci numbers
--By starting with 1 and 2, the first 10 terms will be
--1, 2, 3, 5, 8, 13, 21, 34, 55, 89..
--By considering the terms in the Fibonacci sequence whose values do not
--exceed four million, find the sum of the even-valued terms

fibonaccis = scanl (+) 0 (1:fibonaccis)

problem2 :: Integer -> Integer
problem2 limit = sum $ filter even $ takeWhile (<limit) fibonaccis


--problem 3 largest prime factor
--the prime factor of 13195 are 5, 7, 13, 29
--what is the largest prime factor of the number 600851475143

-- works for input at least 2
primeFactors :: Integer -> [Integer]
primeFactors n 
    | n <= 1 = undefined
    | n <= 3 = [n]
    | otherwise = factorAt n 2
    where 
        factorAt n try -- n are at least 2, try starts with 2
            | try * try > n = [n]
            | r == 0 = try : (factorAt q try)
            | otherwise = factorAt n (if even try then (try+1) else (try+2))
            where (q, r) = n `divMod` try

uniquePrimeFactors = map head . DL.group . primeFactors

problem3 = uniquePrimeFactors

--problem 4 largest palindrome product
--a palindromic number reads the same both ways. The largest palindrome made
--from the product of two 2-digit numbers is 9009 = 91 * 99
--find the largest palindrome made from the product of two 3-digit numbers

toDigits :: Integer -> [Integer]
toDigits n 
    | n < 0 = map negate $ toDigits (negate n)
    | n < 10 = [n]
    | otherwise = let (q, r) = n `divMod` 10 in r : toDigits q

hornerMethod :: [Integer] -> Integer
hornerMethod ns = foldl step 0 ns
    where 
        step acc n = acc*10+n

backwardNum :: Integer -> Integer
backwardNum = hornerMethod . toDigits
   
isPalindromic :: Integer -> Bool
isPalindromic n = n == (backwardNum n)

maybeHead :: [a] -> Maybe a
maybeHead xs
    | null xs = Nothing
    | otherwise = Just (head xs)

problem4' :: (Integer, Integer) -> Integer
problem4' (low, high) 
    | low < high = go 0 seqs seqs
    | otherwise = undefined
    where 
        seqs = reverse [low..high]
        -- find the largest product that is greater than cm
        -- compare x*y first, then all possible x paired with y
        -- then all possible y paired with the first x
        -- then xs and ys
        go cm [] _ = cm
        go cm _ [] = cm
        go cm nx@(x:xs) ny@(y:ys)
            | cm >= p = cm
            | isPalindromic p = p 
            -- now cm < p and p is not, find a smaller p'
            | otherwise = let x1 = cm `pick` h `pick` v
                          in max x1 cm'
            where 
                p = x*y
                isValid x = x > cm && isPalindromic x
                v = maybeHead $ takeWhile isValid $ map (*x) ny
                h = maybeHead $ takeWhile isValid $ map (*y) nx
                cm' = go cm xs ys
                pick :: Ord a => a -> Maybe a -> a
                pick a Nothing = a
                pick a (Just b) = if a >= b then a else b


problem4 :: (Integer, Integer) -> Integer
problem4 (low, high) = maximum $ filter isPalindromic candidates
    where candidates = [x*y | x <- [low..high], y <- [low..high]]


-- problem 5 smallest multiple
-- 2520 is the smallest number that can be divied by each of the numbers
-- from 1 to 10 without remainders
-- what is the smallest positive number that is evenly divisible by all of 
-- the number from 1 to 20

divisible = lcm
evenlyDivisible acc n = if even acc' then acc'*n else acc'*n*2
    where 
        g = gcd acc n
        acc' = acc `div` g

-- get the smallest number that is divisible by each in list
problem5 :: (Integer -> Integer -> Integer) -> [Integer] -> Integer
problem5 f ns = foldl f 1 ns


-- problem 6 sum square difference
-- the sum of the square of the first 10 natural numbers are 
-- 1^2 + 2^2 + ... + 10^2 = 385
-- the square of the sum of the first 10 is
-- (1+2+..+10)^2 = 55^2 = 3025
-- the difference between them is 3025-385=2640
-- find the difference between these two quantities for the first 100 numbers

sumSquare = sum . map (^2)
--linear algorithm
squareSum ns = let s = sum ns in s*s
squareSum' ns = sum $ [x*y | x <- ns, y <- ns] 

problem6 :: [Integer] -> Integer
problem6 ns = (squareSum ns) - (sumSquare ns)

-- problem 7 10001st prime 
-- 2, 3, 5, 7, 11, 13. 6th is 13. What is the 10001st?

isPrime :: Integer -> Bool
isPrime n 
    | n < 0 = undefined
    | n < 2 = False
    | otherwise = (length $ primeFactors n) == 1


--starts at 1st, k=1
kth :: Int -> [a] -> a
kth k
    | k <= 0 = undefined
    | otherwise = head . drop (k-1)

kthPrime' :: Int -> Integer
kthPrime' k = kth k $ filter isPrime [1..]


-- problem 9 Special Pythagorean triplet
-- a Pythagorean triplet is a set of three natural numbers a < b < c for which
-- a^2 + b^2 = c^2
-- there exists exactly one Pythagorean triplet for which a + b + c = 1000
-- find it

type Triangle = (Integer, Integer, Integer)
isPythagoreanTriple :: Triangle -> (Bool, Triangle)
isPythagoreanTriple (a, b, c)
    | a <= 0 || b <= 0 || c <= 0 = (False, (a, b, c))
    | otherwise = (((a'*a' + b'*b') == c'*c'), (a', b', c'))
    where 
        s = a + b + c
        a' = minimum [a, b, c]
        c' = maximum [a, b, c]
        b' = s - a' - c'

findPythagoreanTriple :: Integer -> [Triangle]
findPythagoreanTriple s = [(a, b, s-a-b) | a <- [1..s-2]
                                         , b <- [1..a]
                                         , isValid a b (s-a-b)]
    where 
        isValid a b c = if c <= 0 || c > b 
                        then False
                        else (a*a - b*b) == c*c

problem9 = findPythagoreanTriple 


-- problem 10 summation of primes
-- the sum of primes between 10 is 2, 3, 5, 7, as 17
-- find the sum of all primes below two million

-- this is not a very efficient algorithm
primesTo:: Integer -> [Integer]
primesTo limit = run [2..limit]
    where 
        run [] = []
        run (h:xs) = h : run (xs DL.\\ [2*h,3*h..limit])

-- this is a quadratic algorithm
problem10 :: Integer -> Integer
problem10 limit = sum $ filter isPrime [1..limit]

problem10' limit = sum $ primesTo limit


-- problem 12 highly divisible triangular number
-- the sequence of triangle numbers is generated by adding the natural 
-- numbers. So the 7th triangle number would be 1+2+3+4+5+6+7 = 28. The 
-- first then terms would be 
-- 1, 3, 6, 10, 15, 21, 28, 36, 45, 55
-- let us list the factors of the first seven triangle numbers:
-- 1: 1
-- 3: 1, 3
-- 6: 1, 2, 3, 6
-- 10: 1, 2, 5, 10
-- 15: 1, 3, 5, 15
-- 21: 1, 3, 7, 21
-- 28: 1, 2, 4, 7, 14, 28
-- 28 is the first triangle number to have over five divisors
-- what is the value fo the first triangle number to have over 500 divisors?

numOfFactors :: Integer -> Integer
numOfFactors = product . map (\x -> 1 + fromIntegral (length x)) . factors
    where
        factors = DL.group . primeFactors

problem12 :: Integer -> Integer
problem12 nDiv = head $ dropWhile isNotValid $ triangleNums
    where 
        triangleNums = tail $ scanl (+) 1 [2..]
        isNotValid x = nDiv > (numOfFactors x)
        

-- problem 14 longest collatz sequence
-- the following iterative sequence is defined for the set of positive integers
-- n -> n/2 (n is even)
-- n -> 3*n+1 (n is odd)
-- using the rule and starting with 13, we have
-- 13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
--
-- it can be seen that this sequence (starting at 13 and finishing at 1) 
-- contains 10 items. Although it has not been proved yet (Collatz problem)
-- it is thought that all starting numbers finish at 1
-- which starting number, under one million, produces the longest chain?
-- (NOTE: once the chain starts, the terms are allowed to go above the limit)

collatz :: Integer -> Integer
collatz n
    | n <= 0 = undefined
    | even n = n `div` 2
    | otherwise = 3*n+1 -- odd

uniqueCollatzChain :: Integer -> [Integer]
uniqueCollatzChain start = fst $ run ([], DS.empty) seqs
    where 
        seqs = iterate collatz start
        run (l, s) [] = (l, s)
        run (l, s) (x:xs)
            | x `DS.member` s = (l, s)
            | otherwise = run (x:l, DS.insert x s) xs


-- it stops at 1, always(unproved)
collatzSeq' :: Integer -> [Integer]
collatzSeq' start = run [] seqs
    where
        seqs = iterate collatz start
        run l [] = l
        run l (x:xs)
            | x == 1 = x:l
            | otherwise = run (x:l) xs


takeTill :: Ord a => (a -> Bool) -> [a] -> [a]
takeTill _ [] = []
takeTill f (x:xs)
    | f x = [x]
    | otherwise = x : takeTill f xs


collatzSeq :: Integer -> [Integer]
collatzSeq = takeTill (==1) . iterate collatz

-- returned list is a list of (start, length)
generateCollatzSeq :: Integer 
                   -> DM.Map Integer Integer 
                   -> [(Integer, Integer)]
generateCollatzSeq start m 
    | start `DM.member` m = []
    | otherwise = case nextInSeq of
        Nothing -> zip extras [1..]
        (Just val) -> zip extras [val+1..]
    where 
        seqs = iterate collatz start
        extras = reverse $ 
                 takeTill (\x -> (x == 1) || (DM.member x m)) $ 
                 seqs
        nextInSeq = DM.lookup (collatz $ head extras) m
        

insert' m (k, a) = DM.insert k a m

longestCollatzSeq :: Integer -> (Integer, DM.Map Integer Integer)
longestCollatzSeq limit = DL.foldl' step (0, DM.empty) [1..limit]
    where
        step (longest, m) x = (longest', m')
            where
                extraKeyVals = generateCollatzSeq x m
                m' = DL.foldl' insert' m extraKeyVals
                longest' = if null extraKeyVals 
                           then longest 
                           else max longest (snd $ last extraKeyVals)


problem14 :: Integer -> Integer
problem14 = fst . longestCollatzSeq


-- problem 15 lattice paths
-- starting in the top left corner of a 2 by 2 grid, and only being able
-- move to the right and down. there are exactly 6 routes to the bottome right.
-- RRDD, RDRD, RDDR, DRRD, DRDR, DDRR, which is 4 choose 2, 4*3/2 = 6
-- how many such routes are there through a 20 by 20 grid

-- there are two ways of solving it, 
-- 1) find the unique permuation of 10 Rs and 10 Ds in a string, mathematically
-- it is 40 choose 20, and it can be solve by dynamic programming
-- 2) use dynamic programming to solve the paths to (0, 0), (0, 1), (1, 0), 
-- all the way to (n, m).

-- if one of the n items is captain, you have two ways of choosing k items:
-- 1) without captain, n-1 choose k 2) with captain, n-1 choose k-1
-- n choose k = n-1 choose k + n-1 choose k-1


combination :: Integer -> Integer -> Integer
combination n k
    | n < 0 || k < 0 = undefined
    | k > n = undefined
    | k == 0 = 1
    | k == 1 = n
    | otherwise = (iterate combinationRun [1]) !! n' !! k'
    where
        n' = fromIntegral n 
        k' = fromIntegral k
        -- given a list of number that represents 
        -- n choose 0, choose 1 to choose n, calculate 
        -- n+1 choose 0, choose 1 to choose n+1
        combinationRun :: [Integer] -> [Integer]
        combinationRun ns = [1] ++ (zipWith (+) (init ns) (tail ns)) ++ [1]

problem15 n = combination (2*n) n

--problem 16 power digit sum
--2^15 = 32768 and the sum of its digits is 3+2+7+7+8=26
--what is the sum of digits of the number 2^1000

problem16 = sum . toDigits


--problem 17 number letter counts
--if the numbers 1 to 5 are written out in words, one, two, three, four, five
--then there are 3+3+5+4+4=19 letters used in total. 
--if all the numbers from 1 to 1000 (one thousand) inclusive were written 
--out in words, how many letters would be used? 
--NOTE: do not count spaces or hyphens. 342 (three hundred and forty-two)
--contains 23 letters and 115 (one hundred and fifteen) contain 20 letters. 
--The use of "and" when writing out numbers is in compliance with British use.

toBase :: Integer -> Integer -> [Integer]
toBase base n
    | n < 0 || base < 0 = undefined
    | n < base = [n]
    | otherwise = let (q, r) = n `divMod` base in r : (toBase base q)

numberToEnglish :: Int -> String
numberToEnglish n 
    | n < 0 = undefined
    | n < 20 = uniques !! n
    | n < 100 = let (a1, a0) = n `divMod` 10 
                in (tens !! a1) ++ " " ++ (uniques !! a0)
    | n < 1000 = let (a2, rest) = n `divMod` 100
                     hundreds = uniques !! a2
                 in if rest == 0 
                    then hundreds ++ " hundred"
                    else hundreds ++ " hundred and " ++ numberToEnglish rest
    |otherwise = makeEnglish $ map (numberToEnglish . fromIntegral) $ 
                 toBase 1000 (fromIntegral n)
    where 
        uniques = [ "", "one", "two", "three", "four", "five", "six"
                  , "seven", "eight", "nine", "ten", "eleven", "twelve"
                  , "thirteen", "fourteen", "fifteen", "sixteen", "seventeen"
                  , "eighteen", "nineteen"]
        tens = [ "", "ten", "twenty", "thirty", "fourty", "fifty", "sixty"
               , "seventy", "eighty", "ninety"]
        bigs = ["", "thousand", "million", "billion"]
        makeEnglish thousands = intercalateEnglishNumbers $ 
                                zipWith connect thousands bigs
        connect t b = if null b then t else t ++ " " ++ b
        intercalateEnglishNumbers [] = []
        intercalateEnglishNumbers (x:xs)
            | null x = intercalateEnglishNumbers xs
            | null $ intercalateEnglishNumbers xs = x 
            | otherwise = intercalateEnglishNumbers xs ++ " and " ++ x

letterLength = length . filter DC.isAlpha 

englishNumberLen = letterLength . numberToEnglish

problem17 limit = sum $ map englishNumberLen [1..limit]

--problem 19 Counting Sundays
--You are given the following information, but you may prefer to do some
--research for yourself.
--1900/1/1 was a Monday
--Thirty days has September
--April June and November
--All the rest have thirty one
--saving Feburary alone
--which has twenty-eight, rain or shine
--and on leap years, twenty-nine

--A leap year occurs on any year evenly divisble by 4, but not on a
--century unless it is divisible by 400
--
--how many sunday fell on the first of the month during the twentieth 
--century, 1901/1/1 to 2000/12/31

type Year = Int

isLeapYear :: Year -> Bool
isLeapYear year 
    | year `mod` 4 /= 0 = False
    | year `mod` 100 == 0 = year `mod` 400 == 0
    | otherwise = True

data Month = JAN | FEB | MAR | APR | MAY | JUN 
           | JUL | AUG | SEP | OCT | NOV | DEC
           deriving (Show, Enum, Eq, Ord)

monthDays :: Year -> Month -> Int
monthDays year FEB
    | isLeapYear year = 29
    | otherwise = 28
monthDays _ m 
    | m `elem` [APR, JUN, SEP, NOV] = 30
    | otherwise = 31

nextMonth m = if m == DEC then JAN else succ m

data Weekday = SUN | MON | TUE | WED | THU | FRI | SAT 
               deriving (Show, Enum, Eq, Ord)

nextWeekday wd = if wd == SAT then SUN else succ wd

type Day = Int
data Date = Date { year :: Year, month :: Month, day :: Day } 
            deriving (Show, Eq, Ord)

type CalendarDate = (Date, Weekday)

isEndOfMonth dt = monthDays (year dt) (month dt) == (day dt)

isEndOfYear dt = (month dt == DEC) && isEndOfMonth dt

nextDate dt
    | isEndOfYear dt = Date (y+1) JAN 1
    | isEndOfMonth dt = Date y (succ m) 1
    | otherwise = Date y m (d+1)
    where 
        y = year dt
        m = month dt
        d = day dt

oneWeekLater dt
    | monthDays y m >= d+7 = Date y m (d+7)
    | m == DEC = Date (y+1) JAN (d+7-31)
    | otherwise = Date y m' (d+7-monthDays y m)
    where 
        y = year dt
        m = month dt
        d = day dt
        y' = y+1
        m' = succ m

isFirstOfMonth dt = (day dt) == 1

problem19 = filter isFirstOfMonth $
            takeWhile (< rangeEnd) $ dropWhile (< rangeStart) $ 
            iterate oneWeekLater knownSunday
    where 
        knownSunday = Date 1899 DEC 31
        rangeStart = Date 1901 JAN 1
        rangeEnd = Date 2000 DEC 31


--above is more of an overkill. A faster method would just list days of 
--months as an array and scan it. First time we need to find a Sunday on 1st
--meaning sum % 7 = 6, then we need to find sums of days that are 
--divisible by 7
firstOfNextMonth :: (Year, Month, Weekday) -> (Year, Month, Weekday)
firstOfNextMonth (y, m, wd) = (y', m', wd')
    where 
        f = last . take (((monthDays y m) `mod` 7)+1) . iterate nextWeekday
        (y', m') = if m == DEC then (y+1, JAN) else (y, succ m)
        wd' = f wd

problem19' = filter isSunday $ takeWhile isValid $ dropWhile toSkip $ 
             iterate firstOfNextMonth knownDate
    where 
        knownDate = (1900, JAN, MON)
        toSkip (y, _, _) = y < 1901
        isValid (y, _, _) = y >= 1901 && y <= 2000
        isSunday (_, _, wd) = wd == SUN


--problem 20 factorial digit sum
--n! means n by n-1 by ... by 3 by 2 by 1
--for example 10! = 3628800, digit sum is 27
--find the sum of the digits in number 100!

factorial n = product [1..n]

problem20 = sum . toDigits . factorial
    

--problem 21 amicable numbers
--let d(n) be defined as the sum of proper divisors of n (numbers less than 
--n which divide evenly onto n). If d(a) = b and d(b) = a, where a different 
--from b, then a and b are an amicable pair that each of a and b are called
--amicable numbers
--
--for example the proper divisor of 220 are 1, 2, 4, 5, 10, 11, 22, 44, 55 
--and 110; therefore d(220) = 284. The proper divisor of 284 are 1, 2, 4, 71
--and 142; so d(284) = 220
--
--Evaluate the sum of all amicable numbers under 10000

--function that returns all factors. Inefficient for multiple inputs
properFactors :: Integer -> [Integer]
properFactors n = filter (\x -> n `mod` x == 0) [1..(n `div` 2)]

allProperFactors' limit = DM.fromList $ map calc [1..limit]
    where 
        calc x = (x, (sum $ properFactors x))

amicables' limit = findAmicable $ allProperFactors' limit
    where 
        findAmicable m = DL.foldl' step DS.empty [1..limit]
            where 
                step' :: Integer -> Maybe (Integer, Integer)
                -- given an int, find either 2 amicable numbers or nothing
                step' n = do
                    n' <- DM.lookup n m
                    n'' <- DM.lookup n' m
                    CM.guard (n'' == n && n' /= n)
                    return (n, n'')
                step acc n = case step' n of
                    Nothing -> acc
                    Just (x, y) -> (x `DS.insert`) $ (y `DS.insert`) $ acc

problem21 = sum . amicables'
{--
appendProperFactors :: DS.Set Integer -> Integer -> DS.Set Integer
appendProperFactors s n = s `DS.union` (DS.map (*n) s)

appendProperFactorsM :: DS.Set Integer -> [Integer] -> DS.Set Integer
appendProperFactorsM s ns = foldl appendProperFactors s ns

-- map of proper factors from 1 to limit(input)
allProperFactors :: Integer -> DM.Map Integer (DS.Set Integer)
allProperFactors limit = DL.foldl' step DM.empty [1..limit]
    where 
        step accM n = case DM.lookup n accM of
            Just s -> accM
            Nothing -> insertMap (appendProperFactorsM (DS.fromList [1]) [1..(limit `div` n)])
            where 
                insertMap x = DM.insert n x accM
--}

--problem 22 names scores
--begin a list of names by sorting it into alphabetical order ,then working
--out the alphabetical value for each name, multiply this value by its
--alphabetical position in the list to obtain a name score.
--for example COLIN which is worth 3+15+12+9+14=53, is the 938th name. So 
--its score is 938*53=49714. 
--What is the total of all name scores in the names.txt file?


nameWorth :: String -> Integer
nameWorth = fromIntegral . sum . map asciiVal
    where
        asciiVal c = (DC.digitToInt c)+1-(DC.digitToInt 'A')

totalNameScores :: [String] -> Integer
totalNameScores ns = sum $ zipWith (*) (map nameWorth $ DL.sort ns) [1..]

main = do
    print $ problem1 10  --23
    print $ problem1 1000  --233168
    print $ problem2 4000000 --4613732
    print $ problem3 13195 --5, 7, 13, 29
    print $ problem3 600851475143
    print $ backwardNum 313614
    print $ isPalindromic 1234321
    print $ isPalindromic 123455431
    print $ problem4 (10, 99) -- 9009
    print $ problem4 (100, 999) --906609
    print $ problem5 divisible [1..10] --2520
    --print $ problem5 evenlyDivisible [1..20] --465585120
    print $ problem6 [1..10] --2640
    print $ problem6 [1..100] --25164150
    print $ kthPrime' 10001
    print $ problem9 1000
    print $ problem10 10 --17: sum of primes below 10
    print $ problem10' 1000 --2174734: sum of primes below 6000
    print $ problem12 6 -- 28: first triangle number with 6 factors
    print $ problem12 500 -- 76576500: first triangle number with 500 factors
    print $ longestCollatzSeq 20
    --print $ problem14 1000000 --525
    print $ problem15 20 
    print $ problem16 (2^15) --26
    print $ problem16 (2^1000) --1366
    print $ problem17 1000 --21224
    print $ problem19 
    print $ problem19'
    print $ problem20 100 --648
    print $ amicables' 10000
    print $ problem21 10000

