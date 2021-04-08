-- the idea is to build a purely symbolic arithmetic library without relying
-- on any built-in Int/Integer operations at all. Otherwise, why not just use
-- Integer type, since it is already a big integer type. 

module Arithmetic
  ( D
  , N
  , mkN
  , nzero
  , nFromList
  , I
  , posI
  , negI
  , mkI
  , izero
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

fromInt :: Integer -> Maybe D
fromInt 0 = Just D0
fromInt 1 = Just D1
fromInt 2 = Just D2
fromInt 3 = Just D3
fromInt 4 = Just D4
fromInt 5 = Just D5
fromInt 6 = Just D6
fromInt 7 = Just D7
fromInt 8 = Just D8
fromInt 9 = Just D9
fromInt _ = Nothing

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


----------------- Helper functions for type N ------------------------

mkDRlistFromInt :: Integer -> Maybe [D]
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
divModDlist _ [] = Nothing
divModDlist [] _ = Just ([D0], [D0])
divModDlist ds1 ds2 = Just $ foldl runDiv ([], []) ds1 
  where 
    ds2' = reverse ds2
    runDiv :: ([D], [D]) -> D -> ([D], [D]) 
    runDiv (qr, n) d = (q:qr, dropTrailZero rr)
      where
        (q, rr) = divModRlistForD (d:n) ds2'

-- least signficant digit comes first
-- the quotient will be a single largest possible digit
-- remainder will be produced in a most signficant first format
-- the results also follows least significant first format
divModRlistForD :: [D] -> [D] -> (D, [D])
divModRlistForD num denom = case dropWhile greaterThanNum attempts of
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

instance Show N where
  show (N []) = "0"
  show (N ds) = fmap toChar ds

instance Show N' where
  show (N' ds) = fmap toChar $ NE.toList ds

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

nzero :: N
nzero = N [D0]

nzero' :: N'
nzero' = N' $ D0 NE.:| []

nFromList :: [D] -> N
nFromList xs = N $ dropLeadZero xs


nFromList' :: [D] -> N'
nFromList' xs = case dropLeadZero xs of 
  [] -> N' $ D0 NE.:| []
  xs -> N' $ NE.fromList xs


addN :: N -> N -> N 
addN (N ns) (N ms) = nFromList $ reverse $ addDRlist ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms'


addN' :: N' -> N' -> N'
addN' (N' ns) (N' ms) = nFromList' $ reverse $ addDRlist ns' ms'
  where
    ns' = NE.toList $ NE.reverse ns
    ms' = NE.toList $ NE.reverse ms


subN :: N -> N -> N
subN (N ns) (N ms) = nFromList $ reverse $ subDRlist ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms 

mulN :: N -> N -> N
mulN (N ns) (N ms) = nFromList $ reverse $ mulDRlist ns' ms' 
  where
    ns' = reverse ns
    ms' = reverse ms

divModN :: N -> N -> Maybe (N, N)
divModN (N ns) (N ms) = Bf.bimap toN toN <$> divModDlist ns ms
  where 
    toN = nFromList . dropLeadZero . reverse

--------------------- Integer Number -------------------------
data I = I 
  { iabs :: N 
  , isign :: Bool -- True means positive, 0 can be either way 
  }

posI :: N -> I
posI n = I n True

negI :: N -> I
negI n = I n False

mkI :: String -> Maybe I
mkI ('-':cs) = I <$> mkN cs <*> pure False
mkI cs = I <$> mkN cs <*> pure True

izero :: I
izero = I nzero True

instance Show I where
  show (I n s)
    | s = show n
    | otherwise = if n == nzero then show n else "-" ++ show n

instance Eq I where
  (I n1 s1) == (I n2 s2)
    | isZero1 && isZero2 = True
    | not isZero1 && not isZero2 = if s1 == s2 then n1 == n2 else False 
    | otherwise = False  
    where
      isZero1 = nzero == n1
      isZero2 = nzero == n2

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
      isZero1 = n1 == nzero
      isZero2 = n2 == nzero
      invertOrd GT = LT
      invertOrd LT = GT
      invertOrd EQ = EQ

addI :: I -> I -> I
addI i1@(I n1 s1) i2@(I n2 s2)
  | n1 == nzero = i2
  | n2 == nzero = i1
  | s1 && s2 = I (addN n1 n2) True
  | not s1 && not s2 = I (addN n1 n2) False
  | s1 && not s2 = if n1 < n2 then negI (subN n2 n1) else posI (subN n1 n2)
  | not s1 && s2 = if n1 > n2 then negI (subN n1 n2) else posI (subN n2 n1)

mulI :: I -> I -> I
mulI (I n1 s1) (I n2 s2)
  | n1 == nzero || n2 == nzero = izero
  | s1 == s2 = I (mulN n1 n2) True
  | otherwise = I (mulN n1 n2) False

instance Num I where
  (+) = addI
  (*) = mulI
  abs (I n s) = I n True
  signum i
    | i == izero = izero 
    | isign i = I (N [D1]) True
    | otherwise = I (N [D1]) False
  fromInteger i
    | i == 0 = izero
    | i > 0 = case mkDRlistFromInt i of
      Just n -> I (N $ reverse n) True
      Nothing -> izero
    | otherwise = negate $ fromInteger $ abs i
  negate (I n s)
    | n == nzero = izero
    | otherwise = I n (not s)