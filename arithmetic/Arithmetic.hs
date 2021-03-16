-- the idea is to build a purely symbolic arithmetic library without relying
-- on any built-in Int/Integer operations at all. Otherwise, why not just use
-- Integer type, since it is already a big integer type. 

import Control.Monad (mapM)
import qualified Data.List.NonEmpty as NE 

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

-- for an init value, increase init while decreasing y to stop 
addByCount :: D -> D -> D -> D
addByCount init y stop
  | y <= stop = init
  | otherwise = addByCount (succ init) (pred y) stop


addDD :: D -> D -> D -> (D, D)
addDD x y xcomp
  | y == xcomp = (D1, D0)
  | y > xcomp = let d = addByCount D0 y xcomp in (D1, d) --count y down to xcomp
  | otherwise = let d = addByCount x y D0 in (D0, d) --count y down to D0


-- is the best way just mapping everything out by pattern matching exhaustively?
addD :: D -> D -> (D, D)
addD D0 x = (D0, x)
addD D1 x = addDD D1 x D9
addD D2 x = addDD D2 x D8
addD D3 x = addDD D3 x D7
addD D4 x = addDD D4 x D6
addD D5 x = addDD D5 x D5
addD D6 x = addDD D6 x D4
addD D7 x = addDD D7 x D3
addD D8 x = addDD D8 x D2
addD D9 x = addDD D9 x D1

subD :: D -> D -> (Bool, D) -- Bool indicates borrow
subD x y 
  | x == y = (False, D0)
  | x > y = (False, addByCount D0 x y) -- increase y to x
  | otherwise = let (_, ret) = addD x (addByCount D1 D9 y) in (True, ret)

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
  show (N ds) = fmap toChar ds

instance Show N' where
  show (N' ds) = fmap toChar $ NE.toList ds

instance Eq N where
  (N ds1) == (N ds2) = ds1 == ds2

instance Eq N' where
  (N' ds1) == (N' ds2) = ds1 == ds2


compareDs :: [D] -> [D] -> Ordering
compareDs ds1 ds2 
  | len1 > len2 = GT
  | len1 < len2 = LT
  | otherwise = comp ds1 ds2
  where 
    len1 = length ds1
    len2 = length ds2
    comp [] [] = EQ
    comp [] _ = LT
    comp _ [] = GT
    comp (x:xs) (y:ys)
      | x > y = GT
      | x < y = LT
      | otherwise = comp xs ys


instance Ord N where
  (N ds1) `compare` (N ds2) = ds1 `compare` ds2

instance Ord N' where
  (N' ds1) `compare` (N' ds2) = (NE.toList ds1) `compare` (NE.toList ds2)

dropLeadZero = dropWhile (== D0)

nzero :: N
nzero = N [D0]

isNZero :: N -> Bool
isNZero n = n == nzero

nzero' :: N'
nzero' = N' $ D0 NE.:| []

-- least significant digit comes first
addDs :: [D] -> [D] -> [D]
addDs [] ys = ys
addDs xs [] = xs -- xs is not []
addDs (x:xs) (y:ys) = case tens of 
  D0 -> single:s'
  otherwise -> addDs [single, tens] (D0:s')
  where
    (tens, single) = addD x y
    s' = addDs xs ys


borrowOne :: [D] -> Maybe [D]
borrowOne [] = Nothing
borrowOne (d:ds)
  | d > D0 = Just $ (pred d):ds
  | otherwise = fmap (D9:) (borrowOne ds)

subOne = borrowOne

-- 3201     319  + 11
--  879      87  +  9  
subDs :: [D] -> [D] -> Maybe [D]
subDs [] [] = Just [D0]
subDs xs [] = Just xs
subDs [] ys = Nothing
subDs (x:xs) (y:ys) = case subD x y of
  (False, d) -> fmap (d:) (subDs xs ys)
  (True, d) -> fmap (d:) $ borrowOne xs >>= \n -> subDs n ys


nFromList :: [D] -> N
nFromList xs = N $ dropLeadZero xs


nFromList' :: [D] -> N'
nFromList' xs = case dropLeadZero xs of 
  [] -> N' $ D0 NE.:| []
  xs -> N' $ NE.fromList xs


addN :: N -> N -> N 
addN (N ns) (N ms) = nFromList $ reverse $ addDs ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms'


addN' :: N' -> N' -> N'
addN' (N' ns) (N' ms) = nFromList' $ reverse $ addDs ns' ms'
  where
    ns' = NE.toList $ NE.reverse ns
    ms' = NE.toList $ NE.reverse ms


subN :: N -> N -> Maybe N
subN (N ns) (N ms) = (nFromList . reverse) <$> subDs ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms 

--------------------- Integer Number -------------------------
data I = I 
  { iabs :: N 
  , isign :: Bool -- True means positive, 0 can be either way 
  }

mkI :: String -> Maybe I
mkI ('-':cs) = I <$> mkN cs <*> pure False
mkI cs = I <$> mkN cs <*> pure True

izero :: I
izero = I nzero True

isIZero :: I -> Bool
isIZero (I n s) = isNZero n

isIPositive :: I -> Bool
isIPositive i@(I n s) = not (isIZero i) && s

isINegative :: I -> Bool
isINegative i@(I n s) = not (isIZero i) && not s

instance Show I where
  show (I n s)
    | s = show n
    | otherwise = if n == nzero then show n else "-" ++ show n

instance Eq I where
  i1@(I n1 s1) == i2@(I n2 s2)
    | isZero1 && isZero2 = True
    | not isZero1 && not isZero2 = if s1 == s2 then n1 == n2 else False 
    | otherwise = False  
    where
      isZero1 = isIZero i1
      isZero2 = isIZero i2

invertOrd :: Ordering -> Ordering
invertOrd GT = LT
invertOrd LT = GT
invertOrd EQ = EQ

instance Ord I where
  i1@(I n1 s1) `compare` i2@(I n2 s2)
    | isZero1 && isZero2 = EQ
    | not isZero1 && isZero2 = if isPos1 then GT else LT
    | isZero1 && not isZero2 = if isPos2 then LT else GT
    | isPos1 && isPos2 = n1 `compare` n2
    | isPos1 && isNeg2 = GT
    | isNeg1 && isNeg2 = invertOrd $ n1 `compare` n2
    | otherwise = LT -- isNeg1 && isPos2
    where 
      isZero1 = isIZero i1
      isZero2 = isIZero i2
      isPos1 = isIPositive i1
      isPos2 = isIPositive i2
      isNeg1 = isIPositive i1
      isNeg2 = isIPositive i2

instance Num I where
  i + j = undefined
  i * j = undefined
  abs i = undefined
  signum = undefined
  fromInteger = undefined
  negate = undefined