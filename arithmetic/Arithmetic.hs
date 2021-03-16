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


addDD :: D -> D -> D -> (D, D)
addDD x y xcomp
  | y == xcomp = (D1, D0)
  | y > xcomp = let d = addByCount D0 y xcomp in (D1, d) --count y down to xcomp
  | otherwise = let d = addByCount x y D0 in (D0, d) --count y down to D0
  where
    addByCount :: D -> D -> D -> D
    addByCount init y stop
      | y <= stop = init
      | otherwise = addByCount (succ init) (pred y) stop

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

-- the most significant digit comes first. 
-- the value constructor should not be exported
data N = N [D]
data N' = N' (NE.NonEmpty D)

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

mkN :: String -> Maybe N
mkN cs = (N . dropLeadZero) <$> (mapM fromChar cs)

mkN' :: String -> Maybe N'
mkN' cs = N' <$> (fmap dropLeadZero (mapM fromChar cs) >>= NE.nonEmpty)

nzero :: N
nzero = N [D0]

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
subDs n@(x:xs) m@(y:ys)
  | x >= y = fmap (diff:) (subDs xs ys)
  | otherwise = fmap (diff:) $ borrowOne xs >>= \n -> subDs n ys
  where 
    diff = toEnum $ ((fromEnum x) + 10 - (fromEnum y)) `mod` 10

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