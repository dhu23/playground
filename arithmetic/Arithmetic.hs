-- the idea is to build a purely symbolic arithmetic library without relying
-- on any built-in Int/Integer operations at all. Otherwise, why not just use
-- Integer type, since it is already a big integer type. 

module Arithmetic
  ( D(..)
  , toChar
  , fromChar
  , N
  , mkN
  , subN
  , divModN
  , nZero
  , nOne
  , nFromList
  , I
  , posI
  , negI
  , mkI
  , iZero
  , iOne
  , iMinusOne
  , F
  , posF
  , negF
  ) where

import Control.Monad (mapM)
import qualified Data.List.NonEmpty as NE 
import qualified Data.Bifunctor as Bf (bimap)
import qualified Data.Function as F (on)

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

horner :: Num a => a -> a -> a
horner acc d = acc*10 + d

----------------- Helper functions for type N ------------------------

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

dropLeadZero = dropWhile (== D0)

dropTrailZero = reverse . dropLeadZero . reverse

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

---------------------- Natural number -------------------------
-- the most significant digit comes first. 
-- the value constructor should not be exported
data N = N [D]
data N' = N' (NE.NonEmpty D)

mkN :: String -> Maybe N
mkN cs = (N . dropLeadZero) <$> (mapM fromChar cs)

mkN' :: String -> Maybe N'
mkN' cs = N' <$> (fmap dropLeadZero (mapM fromChar cs) >>= NE.nonEmpty)

nToInteger :: Num a =>  N -> a
nToInteger (N ds) = foldl horner 0 $ fmap (fromIntegral . fromEnum) ds

instance Show N where
  show (N []) = "0"
  show (N ds) = fmap toChar ds

instance Show N' where
  show (N' ds) = fmap toChar $ NE.toList ds

instance Enum N where
  fromEnum (N []) = 0
  fromEnum (N ds) = foldl horner 0 $ fmap fromEnum ds

  -- unfortunately due to the function type dictated by the standard library
  -- we have to write this partial function
  toEnum d
    | d > 0 = case (nFromList . dropLeadZero . reverse) <$> mkDRlistFromInt d of
      Nothing -> undefined
      Just n -> n
    | d == 0 = nZero
    | otherwise = undefined

instance Eq N where
  (N ds1) == (N ds2) = ds1 `eq` ds2
    where
      eq = (==) `F.on` dropLeadZero

instance Eq N' where
  (N' ds1) == (N' ds2) = ds1 == ds2

instance Ord N where
  (N ds1) `compare` (N ds2) = ds1 `comp` ds2
    where
      comp = compareDs `F.on` dropLeadZero

instance Ord N' where
  (N' ds1) `compare` (N' ds2) = (NE.toList ds1) `compare` (NE.toList ds2)

instance Num N where
  (+) = addN
  (*) = mulN
  abs = id
  signum n
    | n == nZero = nZero
    | otherwise = nOne
  fromInteger i 
    | i == 0 = nZero
    | i > 0 = case mkDRlistFromInt i of
      Just n -> nFromList (reverse n)
      Nothing -> nZero
    | otherwise = undefined
  negate = undefined

instance Real N where
  toRational = nToInteger

instance Integral N where
  -- since it is for natural numbers only, quotRem and divMod are identical
  n1 `quotRem` n2 = case n1 `divModN` n2 of
    Nothing -> error "divide by zero for type N"
    Just ret -> ret
  divMod = quotRem -- otherwise it depends on negate function from Num class
  toInteger = nToInteger

nZero :: N
nZero = N [D0]

nOne :: N
nOne = N [D1]

nZero' :: N'
nZero' = N' $ D0 NE.:| []

nFromList :: [D] -> N
nFromList xs = N $ dropLeadZero xs

nFromList' :: [D] -> N'
nFromList' xs = case dropLeadZero xs of 
  [] -> N' $ D0 NE.:| []
  xs -> N' $ NE.fromList xs


addN :: N -> N -> N 
addN (N ns) (N ms) = nFromList $ addDlist ns ms


addN' :: N' -> N' -> N'
addN' (N' ns) (N' ms) = nFromList' $ reverse $ addDRlist ns' ms'
  where
    ns' = NE.toList $ NE.reverse ns
    ms' = NE.toList $ NE.reverse ms


subN :: N -> N -> N
subN (N ns) (N ms) = nFromList $ subDlist ns ms

mulN :: N -> N -> N
mulN (N ns) (N ms) = nFromList $ mulDlist ns ms

divModN0 :: N -> N -> Maybe (N, N)
divModN0 (N ns) (N ms) = Bf.bimap toN toN <$> divModDlist ns ms
  where 
    toN = nFromList . dropLeadZero . reverse

-- this provides a method that is not linearly scaled by the number 
-- of digits difference between the dividend and divisor. 
-- Suppose dividend has 30 extra digits, so there answer is at 10^30 level.
-- It takes on average ~30 long divisions to figure out each digit. 
-- or when we use binary representation of the result, in the format 
-- of b(n)*2^n + b(n-1)*2^(n-1) + ... + b(1)*2^1 + b(0)*2^0
-- b(x) is either 0 or 1
-- there will more digits to work on, however each digit might take fewer 
-- tries to find out
divModN1 :: N -> N -> Maybe (N, N)
divModN1 num denom
  | denom == nZero = Nothing
  | otherwise = undefined

divModN = divModN0

-- when n > 0 and m > 0, gcd(n, m) gives the greatest common divisor
-- when n > 0, gcd(n, 0) = n
-- when both are zero, the above defintion is not enough. 
-- it is commonly defined gcd(0, 0) = 0 to preserve usual identities for GCD
gcdN :: N -> N -> N
gcdN n m
  | n == nZero || m == nZero = nZero
  | n == m = n -- n > 0 and m > 0 and n = m
  | n > m = gcdN (subN n m) m -- Euclid's algorithm
  | otherwise = gcdN (subN m n) n

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

--------------------- Integer Number -------------------------
data I = I N Bool -- True means positive, 0 can be either way 

posI :: N -> I
posI n = I n True

negI :: N -> I
negI n = I n False

mkI :: String -> Maybe I
mkI ('-':cs) = I <$> mkN cs <*> pure False
mkI cs = I <$> mkN cs <*> pure True

iZero :: I
iZero = I nZero True

iPosFromList :: [D] -> I
iPosFromList = posI . nFromList

iNegFromList :: [D] -> I
iNegFromList = negI . nFromList

iOne :: I
iOne = iPosFromList [D1]

iMinusOne :: I
iMinusOne = iNegFromList [D1]

invertOrd :: Ordering -> Ordering
invertOrd GT = LT
invertOrd LT = GT
invertOrd EQ = EQ

instance Show I where
  show (I n s)
    | s = show n
    | otherwise = if n == nZero then show n else "-" ++ show n

instance Enum I where
  fromEnum (I n s) = let val = fromEnum n in if s then val else -val
  toEnum d 
    | d == 0 = iZero
    | d > 0 = posI $ (toEnum d)
    | otherwise = negI $ toEnum (-d)

instance Eq I where
  (I n1 s1) == (I n2 s2)
    | isZero1 && isZero2 = True
    | not isZero1 && not isZero2 = if s1 == s2 then n1 == n2 else False 
    | otherwise = False  
    where
      isZero1 = nZero == n1
      isZero2 = nZero == n2

instance Ord I where
  (I n1 s1) `compare` (I n2 s2)
    | isZero1 && isZero2 = EQ
    | not isZero1 && isZero2 = if s1 then GT else LT
    | isZero1 && not isZero2 = if s2 then LT else GT
    | s1 && s2 = n1 `compare` n2
    | s1 && not s2 = GT
    | not s1 && not s2 = invertOrd $ n1 `compare` n2
    | otherwise = LT --not s1 && s2
    where 
      isZero1 = n1 == nZero
      isZero2 = n2 == nZero

addI :: I -> I -> I
addI i1@(I n1 s1) i2@(I n2 s2)
  | n1 == nZero = i2
  | n2 == nZero = i1
  | s1 && s2 = I (addN n1 n2) True
  | not s1 && not s2 = I (addN n1 n2) False
  | s1 && not s2 = if n1 < n2 then negI (subN n2 n1) else posI (subN n1 n2)
  | not s1 && s2 = if n1 > n2 then negI (subN n1 n2) else posI (subN n2 n1)

mulI :: I -> I -> I
mulI (I n1 s1) (I n2 s2)
  | n1 == nZero || n2 == nZero = iZero
  | s1 == s2 = I (mulN n1 n2) True
  | otherwise = I (mulN n1 n2) False

instance Num I where
  (+) = addI
  (*) = mulI
  abs (I n s) = I n True
  signum i
    | i == 0 = iZero
    | i > 0 = iOne
    | otherwise = iMinusOne 
  fromInteger i
    | i == 0 = iZero
    | i > 0 = I (fromInteger i) True
    | otherwise = negate $ fromInteger $ negate i
  negate (I n s)
    | n == nZero = iZero
    | otherwise = I n (not s)

instance Real I where
  toRational (I n s)
    | n >= nZero = toRational n
    | otherwise = negate $ toRational n

quotRemI :: I -> I -> (I, I)
quotRemI i1@(I n1 s1) i2@(I n2 s2)
  | i2 == iZero = error "divide by zero for type I"
  | i1 == iZero = (iZero, iZero)
  | s1 && s2 = (posI q, posI r)
  | s1 && not s2 = (negI q, posI r)
  | not s1 && s2 = (negI q, negI r)
  | otherwise = (posI q, negI r) -- not s1 && not s2
  where 
    (q, r) = n1 `quotRem` n2

instance Integral I where
  quotRem = quotRemI
  toInteger (I n s)
    | n >= nZero = toInteger n
    | otherwise = negate $ toInteger n

data F = F N N Bool -- numerator, denominator and sign, denom > 0

instance Show F where
  show (F n1 n2 s)
    | n2 == nZero = error "divide by zero in type F"
    | n2 == nOne = show (I n1 s)
    | otherwise = if s then frac else "-" ++ frac
    where 
      frac = show n1 ++ "/" ++ show n2

simplify :: (N, N) -> (N, N)
simplify (num, denom)
  | denom == nZero = (num, denom)
  | divisor == nOne = (num, denom)
  | otherwise = ((num `quot` divisor), (denom `quot` divisor))
  where
    divisor = gcdN num denom

simplifyF :: F -> F
simplifyF (F n d s) = let (n', d') = simplify (n, d) in F n' d' s

constructF :: N -> N -> Bool -> Maybe F
constructF n1 n2 s
  | n2' == nZero = Nothing
  | otherwise = Just $ F n1' n2' s
  where
    (n1', n2') = simplify (n1, n2)

posF :: N -> N -> Maybe F
posF n1 n2 = constructF n1 n2 True

negF :: N -> N -> Maybe F
negF n1 n2  = constructF n1 n2 False

fZero :: F
fZero = F nZero nOne True

fOne :: F
fOne = F nOne nOne True

fMinusOne :: F
fMinusOne = F nOne nOne False

instance Eq F where
  (F n1 d1 s1) == (F n2 d2 s2)
    | d1' == nZero || d2' == nZero = error "divide by zero in type F"
    -- these 4 special cases for performance
    | n1' == nZero && n2' == nZero = True
    | n1' /= nZero && n2' == nZero = False
    | n1' == nZero && n2' /= nZero = False
    | s1 /= s2 = False
    | otherwise = n1' * d2' == n2' * d1' -- comparing n1/d1 vs n2/d2
    where
      (n1', d1') = simplify (n1, d1)
      (n2', d2') = simplify (n2, d2)

instance Ord F where
  (F n1 d1 s1) `compare` (F n2 d2 s2)
    | d1 == nZero || d2 == nZero = error "divide by zero in type F"
    | n1 == nZero && n2 == nZero = EQ
    | n1 /= nZero && n2 == nZero = if s1 then GT else LT
    | n1 == nZero && n2 /= nZero = if s2 then LT else GT
    | s1 && s2 = (n1 * d2) `compare` (n2 * d1)
    | s1 && not s2 = GT
    | not s1 && s2 = LT 
    | otherwise = invertOrd $ (n1 * d2) `compare` (n2 * d1)

addF :: F -> F -> F
addF f1@(F n1 d1 s1) f2@(F n2 d2 s2)
  | d1 == nZero || d2 == nZero = error "divide by zero in type F"
  | n1 == nZero = simplifyF f2
  | n2 == nZero = simplifyF f1
  | s1 && s2 = simplifyF $ F (n1'*d2' + n2'*d1') (d1'*d2') True
  | not s1 && not s2 = simplifyF $ F (n1'*d2' + n2'*d1') (d1'*d2') False
  | s1 && not s2 = 
    let 
      num1 = n1'*d2'
      num2 = n2'*d1'
    in if num1 > num2
      then simplifyF $ F (num1-num2) (d1'*d2') True
      else simplifyF $ F (num2-num1) (d1'*d2') False
  | otherwise = addF f2 f1 -- not s2 && s2
  where 
    (n1', d1') = simplify (n1, d1)
    (n2', d2') = simplify (n2, d2)

mulF :: F -> F -> F
mulF (F n1 d1 s1) (F n2 d2 s2)
  | d1 == nZero || d2 == nZero = error "divide by zero in type F"
  | n1 == nZero || n2 == nZero = fZero
  | otherwise = F num denom (s1 == s2)
  where 
    (num, denom) = simplify (n1*n2, d1*d2)

-- have to control the construction of F to avoid repeatedly checking
instance Num F where
  (+) = addF
  (*) = mulF
  abs (F n d s) = F n d True
  signum (F n d s)
    | n == 0 = 0
    | n > 0 = 1
    | otherwise = (-1)
  fromInteger i 
    | i == 0 = fZero
    | i > 0 = F (fromInteger i) 1 True
    | otherwise = negate $ fromInteger $ negate i
  negate (F n d s)
    | n == 0 = fZero
    | otherwise = F n d (not s)