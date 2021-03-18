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
subOneRlist :: [D] -> Maybe [D]
subOneRlist [] = Nothing
subOneRlist (d:ds)
  | d > D0 = Just $ (pred d):ds
  | otherwise = fmap (D9:) (subOneRlist ds)

-- digits in reversed order, least signficant digit comes first.
-- result is at least 0. Otherwise defining integer number subtraction
-- can be very tricky and ugly
subDRlist :: [D] -> [D] -> [D]
subDRlist xs [] = [D0]
subDRlist [] ys = [D0]
subDRlist (x:xs) (y:ys) = case subD x y of
  (False, d) -> fmap (d:) (subDRlist xs ys)
  (True, d) -> fmap (d:) $ subOneRlist xs >>= \n -> subDRlist n ys

mulD :: D -> D -> (D, D)
mulD D0 _ = (D0, D0)
mulD D1 x = (D0, x)
mulD D2 x = addD x x
mulD D3 x = addDD (mulD D2 x) (mulD D1 x)
mulD D4 x = let r = mulD D2 x in addDD r r
mulD D5 x = addDD (mulD D4 x) (mulD D1 x)
mulD D6 x = let r = mulD D3 x in addDD r r
mulD D7 x = addDD (mulD D4 x) (mulD D3 x)
mulD D8 x = let r = mulD D4 x in addDD r r
mulD D9 x = addDD (mulD D8 x) (mulD D1 x)

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


subN :: N -> N -> Maybe N
subN (N ns) (N ms) = (nFromList . reverse) <$> subDRlist ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms 

mulN :: N -> N -> N
mulN (N ns) (N ms) = nFromList $ reverse $ mulDRlist ns' ms' 
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

addI :: I -> I -> I
addI i1@(I n1 s1) i2@(I n2 s2)
  | isZero1 = i2
  | isZero2 = i1
  | isPos1 && isPos2 = I (addN n1 n2) True
  | isNeg1 && isNeg2 = I (addN n1 n2) False
  | isPos1 && isNeg2 = if n1 >= n2 then I (subN n1 n2)
  where
    isZero1 = isIZero i1
    isZero2 = isIZero i2
    isPos1 = isIPositive i1
    isPos2 = isIPositive i2
    isNeg1 = isIPositive i1
    isNeg2 = isIPositive i2

instance Num I where
  i + j = addI
  i * j = undefined
  abs i = undefined
  signum = undefined
  fromInteger = undefined
  negate = undefined
