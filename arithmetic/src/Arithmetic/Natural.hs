module Arithmetic.Natural
  ( N
  ) where


import Arithmetic.Digit 
  ( D(..)
  , fromChar
  , toChar
  , mkDlistFromInt
  , addDlist
  , subDlist
  , mulDlist
  , divModDlist
  , compareDs
  , dropLeadZero
  )


import qualified Data.Function as F (on)
import qualified Data.Bifunctor as Bf (bimap)

-- the most significant digit comes first. 
-- the value constructor should not be exported
-- We prefer [] over [D0], though identical
-- due to the logic to drop leading zeros
newtype N = N { getDs :: [D] } deriving Show


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
  compare = compareDs `F.on` getDs

addN :: N -> N -> N 
addN n1 n2 = mkN $ addDlist (getDs n1) (getDs n2)

subN :: N -> N -> N
subN n1 n2 = mkN $ subDlist (getDs n1) (getDs n2)

mulN :: N -> N -> N
mulN n1 n2 = mkN $ mulDlist (getDs n1) (getDs n2)

divModN :: N -> N -> Maybe (N, N)
divModN n1 n2 = Bf.bimap mkN mkN <$> divModDlist (getDs n1) (getDs n2)
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


-- the fundamental function to create N. This constructor is responsible for
-- making sure that there is no leading D0 contained in the list
mkN :: [D] -> N
mkN = N . dropLeadZero


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



