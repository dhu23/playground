module Arithmetic.Natural
  ( N
  , SumN(..)
  , ProdN(..)
  , fromStr
  , toStr
  , n'0
  , n'1
  , mkN
  , addN
  , subN
  , mulN
  , divModN
  , quotRemN
  , gcdN
  , lcmN
  , nToInteger
  , nFromInteger
  ) where


import Arithmetic.Digit 
  ( D(..)
  , fromChar
  , toChar
  , fromInt
  , addD
  , subD
  , mulD
  )


import qualified Data.Function as F (on)
import qualified Data.Bifunctor as Bf (bimap)
import qualified Control.Monad as CM (mapM)
import Arithmetic.Readable 
  ( Readable
  , fromStr
  , toStr
  )

-- the most significant digit comes first. 
-- the value constructor should not be exported
-- We prefer [] over [D0], though identical
-- due to the logic to drop leading zeros
newtype N = N { getDs :: [D] } deriving Show

-- the fundamental function to create N. This constructor is responsible for
-- making sure that there is no leading D0 contained in the list
mkN :: [D] -> N
mkN = N . dropLeadZero


n'0 = mkN [D0]
n'1 = mkN [D1]
n'2 = mkN [D2] 
n'3 = mkN [D3]
n'4 = mkN [D4]
n'5 = mkN [D5]
n'6 = mkN [D6]
n'7 = mkN [D7]
n'8 = mkN [D8]
n'9 = mkN [D9]
n'10 = mkN [D1, D0]


instance Readable N where
  fromStr = fmap mkN . CM.mapM fromChar
  toStr = show . nToInteger


instance Eq N where
  (==) = (==) `F.on` getDs

instance Enum N where
  fromEnum n = case getDs n of
    [] -> 0
    ds -> foldl horner 0 $ fmap fromEnum ds
  -- unfortunately due to the function type dictated by the standard library
  -- we have to write this partial function
  toEnum d
    | d > 0 = case mkN <$> mkDlistFromInt d of
      Nothing -> undefined -- really shouldn't happen
      Just n -> n
    | d == 0 = n'0 
    | otherwise = undefined -- does not support negative value

instance Ord N where
  compare = compareDlist `F.on` getDs


newtype SumN = SumN N deriving (Eq, Show)

instance Semigroup SumN where
  (SumN x) <> (SumN y) = SumN $ x `addN` y

-- instance Semigroup SumN where
instance Monoid SumN where
  mempty = SumN n'0

newtype ProdN = ProdN N deriving (Eq, Show)

instance Semigroup ProdN where
  (ProdN x) <> (ProdN y) = ProdN $ x `mulN` y

instance Monoid ProdN where
  mempty = ProdN n'1


addN :: N -> N -> N 
addN n1 n2 = mkN $ addDlist (getDs n1) (getDs n2)

subN :: N -> N -> N
subN n1 n2 = mkN $ subDlist (getDs n1) (getDs n2)

mulN :: N -> N -> N
mulN n1 n2 = mkN $ mulDlist (getDs n1) (getDs n2)

divModN :: N -> N -> Maybe (N, N)
divModN n1 n2 = Bf.bimap mkN mkN <$> divModDlist (getDs n1) (getDs n2)

quotRemN :: N -> N -> Maybe (N, N)
quotRemN = divModN

-- when n > 0 and m > 0, gcd(n, m) gives the greatest common divisor
-- when n > 0, gcd(n, 0) = n
-- when both are zero, the above defintion is not enough. 
-- it is commonly defined gcd(0, 0) = 0 to preserve usual identities for GCD
gcdN' :: N -> N -> N
gcdN' n m
  | n == n'0 || m == n'0 = n'0
  | n == m = n -- n > 0 and m > 0 and n = m
  | n > m = gcdN' (subN n m) m -- Euclid's algorithm slower when n >> m
  | otherwise = gcdN' (subN m n) n

gcdN :: N -> N -> N
gcdN n m
  | n == n'0 = m
  | m == n'0 = n
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
  | n == n'0 || m == n'0 = n'0 
  | otherwise = case divModN (mulN n m) (gcdN n m) of
    Nothing -> n'0 -- this means both n and m are zero, won't happen
    Just (q, _) -> q


nToInteger :: Num a =>  N -> a
nToInteger (N ds) = foldl horner 0 $ fmap (fromIntegral . fromEnum) ds

nFromInteger :: Integer -> Maybe N
nFromInteger i 
  | i == 0 = Just n'0 
  | i > 0 = case mkDlistFromInt i of 
    Just n -> Just $ mkN n
    Nothing -> Just n'0 
  | otherwise = Nothing

nDigitCount :: N -> N
nDigitCount n = if null ds then n'1 else foldr count n'0 ds
  where 
    ds = dropLeadZero (getDs n)
    count :: D -> N -> N
    count _ x = addN x n'1 

----------------- Helper functions for type N ------------------------

horner :: Num a => a -> a -> a
horner acc d = acc*10 + d


dropLead :: Eq a => a -> [a] -> [a]
dropLead a = dropWhile (== a)

dropTrail :: Eq a => a -> [a] -> [a]
dropTrail a = reverse . dropLead a . reverse


-- most significant digit first
mkDlistFromInt :: Integral a => a -> Maybe [D]
mkDlistFromInt = fmap reverse . mkDRlistFromInt'


--most significant digit first
addDlist :: [D] -> [D] -> [D]
addDlist ds1 ds2 = reverse $ ds1 `add'` ds2
  where
    add' = addDRlist `F.on` reverse


--most significant digit first
subDlist :: [D] -> [D] -> [D]
subDlist ds1 ds2 = dropLeadZero $ reverse $ ds1 `sub'` ds2
  where
    sub' = subDRlist `F.on` reverse


-- most significant digit first
mulDlist :: [D] -> [D] -> [D]
mulDlist ds1 ds2 = reverse $ ds1 `mul'` ds2
  where
    mul' = mulDRlist `F.on` reverse

compareDlist :: [D] -> [D] -> Ordering
compareDlist ds1 ds2 
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
divModDlist ds1 ds2 = Just $ Bf.bimap reverse' reverse' $ foldl runDiv ([], []) ds1
  where
    reverse' = dropLeadZero . reverse
    ds2' = reverse ds2
    runDiv :: ([D], [D]) -> D -> ([D], [D]) 
    runDiv (qr, n) d = (q:qr, dropTrailZero rr)
      where
        (q, rr) = divModRlistForD0 (d:n) ds2'


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


dropLeadZero = dropLead D0

dropTrailZero = dropTrail D0


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
    greaterThanNum (_, x) = (reverse x) `compareDlist` (reverse num) == GT

-- least significant digit first
mkDRlistFromInt' :: Integral a => a -> Maybe [D]
mkDRlistFromInt' x
  | x <= 0 = Just [D0]
  | x < 10 = sequence [fromInt x] -- Just [D]
  | otherwise = 
    let (q, r) = x `divMod` 10
    in (:) <$> fromInt r <*> mkDRlistFromInt' q


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


