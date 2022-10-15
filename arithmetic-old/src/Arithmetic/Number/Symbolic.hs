-- the idea is to build a purely symbolic arithmetic library without relying
-- on any built-in Int/Integer operations at all. Otherwise, why not just use
-- Integer type, since it is already a big integer type. 

module Arithmetic.Number.Symbolic
  ( D(..)
  , toChar
  , fromChar
  , N -- not exposing N internals to prevent pattern match. use mkN
  , mkN
  , addN
  , subN
  , mulN
  , divModN
  , quotRemN
  , gcdN
  , lcmN
  , nZero
  , nOne
  , nTwo
  , nThree
  , nFour
  , nFive
  , nSix
  , nSeven
  , nEight
  , nNine
  , nTen
  ) where

import qualified Data.Function as F (on)
import qualified Data.Bifunctor as Bf (bimap)

-------------------- Natural Number ----------------------------------
-- the most significant digit comes first. 
-- the value constructor should not be exported
newtype N = N { getDs :: [D] } deriving Show

instance Eq N where
  (==) = (==) `F.on` getDs

instance Enum N where
  fromEnum n = case getDs n of
    [] -> 0
    ds -> foldl horner 0 $ fmap fromEnum ds
  -- unfortunately due to the function type dictated by the standard library
  -- we have to write this partial function
  toEnum d
    | d > 0 = case (mkN . dropLeadZero . reverse) <$> mkDRlistFromInt d of
      Nothing -> undefined -- really shouldn't happen
      Just n -> n
    | d == 0 = nZero
    | otherwise = undefined -- does not support negative value

instance Ord N where
  compare = compareDs `F.on` getDs

addN :: N -> N -> N 
addN n1 n2 = mkN $ addDlist (getDs n1) (getDs n2)

subN :: N -> N -> N
subN n1 n2 = mkN $ subDlist (getDs n1) (getDs n2)

mulN :: N -> N -> N
mulN n1 n2 = mkN $ mulDlist (getDs n1) (getDs n2)

divModN :: N -> N -> Maybe (N, N)
divModN n1 n2 = Bf.bimap toN toN <$> divModDlist (getDs n1) (getDs n2)
  where 
    toN = mkN . reverse

quotRemN :: N -> N -> Maybe (N, N)
quotRemN = divModN

-- when n > 0 and m > 0, gcd(n, m) gives the greatest common divisor
-- when n > 0, gcd(n, 0) = n
-- when both are zero, the above defintion is not enough. 
-- it is commonly defined gcd(0, 0) = 0 to preserve usual identities for GCD
gcdN' :: N -> N -> N
gcdN' n m
  | n == nZero || m == nZero = nZero
  | n == m = n -- n > 0 and m > 0 and n = m
  | n > m = gcdN' (subN n m) m -- Euclid's algorithm slower when n >> m
  | otherwise = gcdN' (subN m n) n

gcdN :: N -> N -> N
gcdN n m
  | n == nZero = m
  | m == nZero = n
  | n == m = n
  | n > m = case divModN n m of  -- Euclidean algorithm that's much faster
    Nothing -> n -- when m is 0
    Just (q, r) -> gcdN m r
  | otherwise = gcdN m n

-- A = a * gcd(A, B)
-- B = b * gcd(A, B)
-- lcm = a * b * gcd(A, B)
-- lcm(0, a) = 0 for all a
lcmN :: N -> N -> N
lcmN n m
  | n == nZero || m == nZero = nZero
  | otherwise = case divModN (mulN n m) (gcdN n m) of
    Nothing -> nZero -- this means both n and m are zero, won't happen
    Just (q, _) -> q


-- the fundamental function to create N. This constructor is responsible for
-- making sure that there is no leading D0 contained in the list
mkN :: [D] -> N
mkN = N . dropLeadZero

nZero = mkN []
nOne = mkN [D1]
nTwo = mkN [D2] 
nThree = mkN [D3]
nFour = mkN [D4]
nFive = mkN [D5]
nSix = mkN [D6]
nSeven = mkN [D7]
nEight = mkN [D8]
nNine = mkN [D9]
nTen = mkN [D1, D0]


nToInteger :: Num a =>  N -> a
nToInteger (N ds) = foldl horner 0 $ fmap (fromIntegral . fromEnum) ds

nFromInteger :: Integer -> Maybe N
nFromInteger i 
  | i == 0 = Just nZero
  | i > 0 = case mkDRlistFromInt i of 
    Just n -> Just $ mkN (reverse n)
    Nothing -> Just nZero
  | otherwise = Nothing

nDigitCount :: N -> N
nDigitCount n = if null ds then nOne else foldr count nZero ds
  where 
    ds = dropLeadZero (getDs n)
    count :: D -> N -> N
    count _ x = addN x nOne

------------------- digit D type --------------------------------------

data D
  = D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8 | D9 
  deriving (Enum, Bounded, Eq, Ord, Show)


fromChar :: Char -> Maybe D
fromChar '0' = Just D0
fromChar '1' = Just D1
fromChar '2' = Just D2
fromChar '3' = Just D3
fromChar '4' = Just D4
fromChar '5' = Just D5
fromChar '6' = Just D6
fromChar '7' = Just D7
fromChar '8' = Just D8
fromChar '9' = Just D9
fromChar _ = Nothing


toChar :: D -> Char
toChar D0 = '0'
toChar D1 = '1'
toChar D2 = '2'
toChar D3 = '3'
toChar D4 = '4'
toChar D5 = '5'
toChar D6 = '6'
toChar D7 = '7'
toChar D8 = '8'
toChar D9 = '9'


fromInt :: Integral a => a -> Maybe D
fromInt d
  | d >= 0 && d <= 9 = Just $ toEnum $ fromIntegral d
  | otherwise = Nothing


----------------- Helper functions for type N ------------------------
dropLead :: Eq a => a -> [a] -> [a]
dropLead a = dropWhile (== a)

dropTrail :: Eq a => a -> [a] -> [a]
dropTrail a = reverse . dropLead a . reverse

horner :: Num a => a -> a -> a
horner acc d = acc*10 + d


-- least significant digit first
mkDRlistFromInt :: Integral a => a -> Maybe [D]
mkDRlistFromInt x
  | x <= 0 = Just [D0]
  | x < 10 = sequence [fromInt x] -- Just [D]
  | otherwise = 
    let (q, r) = x `divMod` 10
    in (:) <$> fromInt r <*> mkDRlistFromInt q

-- for an init value, increase init while decreasing y to stop 
addByCount :: D -> D -> D -> D
addByCount init y stop
  | y <= stop = init
  | otherwise = addByCount (succ init) (pred y) stop


runAddByCount :: D -> D -> D -> (D, D)
runAddByCount x y xcomp
  | y == xcomp = (D1, D0)
  | y > xcomp = let d = addByCount D0 y xcomp in (D1, d) --count y down to xcomp
  | otherwise = let d = addByCount x y D0 in (D0, d) --count y down to D0


-- is the best way just mapping everything out by pattern matching exhaustively?
addD :: D -> D -> (D, D)
addD D0 x = (D0, x)
addD D1 x = runAddByCount D1 x D9
addD D2 x = runAddByCount D2 x D8
addD D3 x = runAddByCount D3 x D7
addD D4 x = runAddByCount D4 x D6
addD D5 x = runAddByCount D5 x D5
addD D6 x = runAddByCount D6 x D4
addD D7 x = runAddByCount D7 x D3
addD D8 x = runAddByCount D8 x D2
addD D9 x = runAddByCount D9 x D1

subD :: D -> D -> (Bool, D) -- Bool indicates borrow
subD x y
  | x == y = (False, D0)
  | x > y = (False, addByCount D0 x y) -- increase y to x
  | otherwise = let (_, ret) = addD x (addByCount D1 D9 y) in (True, ret)

addDD :: (D, D) -> (D, D) -> (D, D) -- hundreds are ignored
addDD (ten1, single1) (ten2, single2) = (ten, single)
  where
    (tenCarry, single) = addD single1 single2
    (_, ten') = addD ten1 ten2
    (_, ten) = addD ten' tenCarry


-- least significant digit comes first. Rlist = reversed list
addDRlist :: [D] -> [D] -> [D]
addDRlist [] ys = ys
addDRlist xs [] = xs -- xs is not []
addDRlist (x:xs) (y:ys) = case tens of 
  D0 -> single:s'
  otherwise -> addDRlist [single, tens] (D0:s')
  where
    (tens, single) = addD x y
    s' = addDRlist xs ys


--most significant digit first
addDlist :: [D] -> [D] -> [D]
addDlist ds1 ds2 = reverse $ ds1 `add'` ds2
  where
    add' = addDRlist `F.on` reverse

sumDRlists :: [[D]] -> [D]
sumDRlists = foldr addDRlist [D0]

-- least significant digit comes first
-- the result will be at least zero
subOneRlist :: [D] -> [D]
subOneRlist [] = [D0]
subOneRlist [D0] = [D0]
subOneRlist (d:ds)
  | d > D0 = (pred d):ds
  | otherwise = D9:(subOneRlist ds)

-- digits in reversed order, least signficant digit comes first.
-- result is at least 0. Otherwise defining integer number subtraction
-- can be very tricky and ugly
subDRlist :: [D] -> [D] -> [D]
subDRlist xs [] = xs
subDRlist [] ys = [D0]
subDRlist (x:xs) (y:ys) = case subD x y of
  (False, d) -> d:(subDRlist xs ys)
  (True, d) -> d:(subDRlist (subOneRlist xs) ys)

--most significant digit first
subDlist :: [D] -> [D] -> [D]
subDlist ds1 ds2 = reverse $ ds1 `sub'` ds2
  where
    sub' = subDRlist `F.on` reverse

mulD :: D -> D -> (D, D)
mulD D0 _ = (D0, D0)  -- 0 addition
mulD D1 x = (D0, x)   -- 0 addition
mulD D2 x = addD x x  -- 1
mulD D3 x = addDD (mulD D2 x) (mulD D1 x)  -- 2 additions
mulD D4 x = let r = mulD D2 x in addDD r r -- 2 additions
mulD D5 x = addDD (mulD D4 x) (mulD D1 x)  -- 3 additions
mulD D6 x = let r = mulD D3 x in addDD r r -- 3 additions
mulD D7 x = addDD (mulD D6 x) (mulD D1 x)  -- 4 additions
mulD D8 x = let r = mulD D4 x in addDD r r -- 3 additions
mulD D9 x = addDD (mulD D8 x) (mulD D1 x)  -- 4 additions

-- least significant digit comes first
mulDRlistByD :: [D] -> D -> [D]
mulDRlistByD [] m = []
mulDRlistByD (d:ds) m = case ten of 
  D0 -> single:(mulDRlistByD ds m)
  _ -> single:(addDRlist [ten] (mulDRlistByD ds m))
  where
    (ten, single) = mulD d m


-- least significant digit comes first
mulDRlist :: [D] -> [D] -> [D]
mulDRlist ns ms = sumDRlists rows
  where
    -- [7, 9, 8] -> [(7, []), (9, [7]), (8, [9, 7])]
    helper :: [a] -> [(a, [a])] 
    helper xs = reverse $ helper2 $ reverse xs
      where 
        helper2 :: [a] -> [(a, [a])]
        helper2 [] = []
        helper2 (a:as) = (a, as):(helper2 as)
    mul' ms (d, countList) = mulDRlistByTens (mulDRlistByD ms d) countList
    rows = map (mul' ns) $ helper ms
    -- the first argument is in least significant digit format
    mulDRlistByTens :: [D] -> [a] -> [D]
    mulDRlistByTens x [] = x
    mulDRlistByTens x (_:rest) = mulDRlistByTens (D0:x) rest

-- most significant digit first
mulDlist :: [D] -> [D] -> [D]
mulDlist ds1 ds2 = reverse $ ds1 `mul'` ds2
  where
    mul' = mulDRlist `F.on` reverse

dropLeadZero = dropLead D0

dropTrailZero = dropTrail D0

compareDs :: [D] -> [D] -> Ordering
compareDs ds1 ds2 
  | len1 > len2 = GT
  | len1 < len2 = LT
  | otherwise = ds1' `compare` ds2' -- use list lexicographical comparison
  where 
    ds1' = dropLeadZero ds1
    ds2' = dropLeadZero ds2
    len1 = length ds1'
    len2 = length ds2'

-- most significant digit comes first
divModDlist :: [D] -> [D] -> Maybe ([D], [D])
divModDlist _ ds2
  | length (dropLeadZero ds2) == 0 = Nothing
divModDlist [] _ = Just ([D0], [D0])
divModDlist ds1 ds2 = Just $ foldl runDiv ([], []) ds1 
  where 
    ds2' = reverse ds2
    runDiv :: ([D], [D]) -> D -> ([D], [D]) 
    runDiv (qr, n) d = (q:qr, dropTrailZero rr)
      where
        (q, rr) = divModRlistForD0 (d:n) ds2'

-- least signficant digit comes first
-- the quotient will be a single largest possible digit
-- remainder will be produced in a most signficant first format
-- the results also follows least significant first format
-- 
-- this method resembles a manual long division process 
-- suppose we have x digits in the dividend and y digits (y << x) in the divisor
-- so the entire process is linear to the (x-y) as that's the number 
-- of long division we have to perform. 
--

divModRlistForD0 :: [D] -> [D] -> (D, [D])
divModRlistForD0 num denom = case dropWhile greaterThanNum attempts of
  [] -> (D0, num)
  ((i, val):_) -> (i, subDRlist num val)
  where 
    mul' = mulDRlistByD denom 
    attempts = fmap (\x -> (x, mul' x)) $ reverse [D1 .. D9]
    greaterThanNum (_, x) = (reverse x) `compareDs` (reverse num) == GT

