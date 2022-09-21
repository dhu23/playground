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
  , divModRlistForD0
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
newtype N = N { getDs :: DList } deriving Show

type DList = [D] -- sequence of digits with most significant digit first
type DRList = [D] -- sequence of digits with least significant digit first

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
addN (N dxs) (N dys) = let add' = addDRlist `F.on` reverse
  in mkN $ reverse $ dxs `add'` dys

subN :: N -> N -> N
subN (N dxs) (N dys) = let sub' = subDRlist `F.on` reverse
  in mkN $ reverse $ dxs `sub'` dys

mulN :: N -> N -> N
mulN (N dxs) (N dys) = let mul' = mulDRlist `F.on` reverse
  in mkN $ reverse $ dxs `mul'` dys

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
mkDlistFromInt :: Integral a => a -> Maybe DList 
mkDlistFromInt = fmap reverse . mkDRlistFromInt'


-- least significant digit comes first. Rlist = reversed list
addDRlist :: DRList -> DRList -> DRList 
addDRlist [] ys = ys
addDRlist xs [] = xs -- xs is not []
addDRlist (x:xs) (y:ys) = case tens of 
  D0 -> single:s'
  otherwise -> addDRlist [single, tens] (D0:s')
  where
    (tens, single) = addD x y
    s' = addDRlist xs ys

-- least significant digit comes first
-- the result will be at least zero
subOneDRlist :: DRList -> DRList
subOneDRlist [] = []
subOneDRlist (D0:ds) = D9:(subOneDRlist ds)
subOneDRlist (d:ds) = (pred d):ds

-- digits in reversed order, least signficant digit comes first.
-- result is at least 0. Otherwise defining integer number subtraction
-- can be very tricky and ugly
subDRlist :: DRList -> DRList -> DRList 
subDRlist xs [] = xs
subDRlist [] ys = []
subDRlist (x:xs) (y:ys) = case subD x y of
  (False, d) -> d:(subDRlist xs ys)
  (True, d) -> d:(subDRlist (subOneDRlist xs) ys)

-- least significant digit comes first
mulDRlistByD :: DRList -> D -> DRList
mulDRlistByD [] m = []
mulDRlistByD (d:ds) m = case ten of 
  D0 -> single:(mulDRlistByD ds m)
  _ -> single:(addDRlist [ten] (mulDRlistByD ds m))
  where
    (ten, single) = mulD d m

mulDRlistByTen :: DRList -> DRList
mulDRlistByTen [] = []
mulDRlistByTen ds = D0:ds


---    1245  DList
--- x   987  DList
--------------------
-- ==  1245 * 9 * 100 + 1245 * 8 * 10 + 1245 * 7 * 1
-- should be interpretted as
--     5421  DRList
-- x    987  DList
-- -----------------
-- ==  ((0 * 10 + 5421(dr) * 9) * 10 + 5421(dr) * 8) * 10 + 5421(dr) * 7 (NOT foldr)
-- or 
--     5421  DRList
-- x    789  DRList
-- -----------------
-- ==  ((0 * 10 + 5421(dr) * 9) * 10 + 5421(dr) * 8) * 10 + 5421(dr) * 7 (now foldr)
mulDRlist :: DRList -> DRList -> DRList
mulDRlist [] _ = []
mulDRlist _ [] = []
mulDRlist xs ys = foldr run [] ys
  where
    run d acc = (mulDRlistByTen acc) `addDRlist` (xs `mulDRlistByD` d)

compareDlist :: DList -> DList -> Ordering
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
divModDlist :: DList -> DList -> Maybe (DList, DList)
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
-- 1462 % 463 
--    1  % 463 = 0 ...   1  0x463 =    0
--   14  % 463 = 0 ...  14  0x463 =    0
--  146  % 463 = 0 ... 146  0x463 =    0
-- 1462  % 463 = 3 ...  73  3x463 = 1389
--  730  % 463 = 1 ... 267  1x463 =  463
-- 2670  % 463 = 5 ... 355  5x463 = 2315
-- 3550  % 463 = 7 ... 309  7x463 = 3241

divModRlistForD0 :: DRList -> DRList -> (D, DRList)
divModRlistForD0 num denom = case dropWhile greaterThanNum attempts of
  [] -> (D0, num)
  ((i, val):_) -> (i, subDRlist num val)
  where 
    mul' = mulDRlistByD denom 
    attempts = fmap (\x -> (x, mul' x)) $ reverse [D1 .. D9]
    greaterThanNum (_, x) = (reverse x) `compareDlist` (reverse num) == GT


-- least significant digit first
mkDRlistFromInt' :: Integral a => a -> Maybe DRList
mkDRlistFromInt' x
  | x <= 0 = Just [D0]
  | x < 10 = sequence [fromInt x] -- Just [D]
  | otherwise = 
    let (q, r) = x `divMod` 10
    in (:) <$> fromInt r <*> mkDRlistFromInt' q


dropLeadZero = dropLead D0

dropTrailZero = dropTrail D0
