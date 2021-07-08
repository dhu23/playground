
module Arithmetic
  ( D(..) -- exposing the 10 digits
  , toChar
  , fromChar
  , N -- not exposing N internals. N is constructed only from mkN
  , mkN
  , addN
  , subN
  , mulN
  , divModN
  , nZero 
  , nOne
  , Z
  , posZFromN
  , negZFromN
  , mkZ
  , iZero
  , iOne
  , iMinusOne
  , DF
  , constructDF
  , F
  , debugF
  , getDenom
  , constructF
  , posFFromNs
  , negFFromNs
  , fFromZ
  ) where

import Control.Monad (mapM)
import qualified Data.List.NonEmpty as NE 
import qualified Data.Bifunctor as Bf (bimap)
import qualified Data.Function as F (on)
import qualified Data.List.Split as S (splitOn)

import Debug.Trace



instance HumanReadable D where
  fromString s = case dropLead ' ' $ dropTrail ' ' s of 
    [x] -> fromChar x
    _ -> Nothing

  toString = return . toChar



----------------- alternative implementation for Natural Number ------------
-- data N' = N' (NE.NonEmpty D)
-- 
-- newtype N0 = N0 { getN :: Maybe [D] }
-- n0FromMaybeDs = N0
-- 
-- mkN0 :: String -> N0
-- mkN0 cs = N0 $ mkN cs >>= \(N ds) -> return ds 
-- 
-- mkN' :: String -> Maybe N'
-- mkN' cs = N' <$> (fmap dropLeadZero (mapM fromChar cs) >>= NE.nonEmpty)
-- 
-- instance Show N0 where
--   show n = case getN n of 
--     Nothing -> "NaN"
--     Just [] -> "0"
--     Just ds -> fmap toChar ds
-- 
-- instance Show N' where
--   show (N' ds) = fmap toChar $ NE.toList ds
-- 
-- instance Enum N0 where
--   fromEnum n = case getN n of
--     Nothing -> -1
--     Just ds -> foldl horner 0 $ fmap fromEnum ds
-- 
--   toEnum d
--     | d > 0 = case (dropLeadZero . reverse) <$> mkDRlistFromInt d of
--       Nothing -> N0 Nothing --this really shouldn't happen
--       Just [] -> N0 Nothing --this really shouldn't happen
--       n -> N0 $ n
--     | d == 0 = N0 $ Just [D0]
--     | otherwise = N0 Nothing
-- 
-- instance Eq N0 where
--   (N0 Nothing) == (N0 Nothing) = True
--   (N0 Nothing) == _ = False
--   (N0 (Just _)) == (N0 Nothing) = False
--   (N0 (Just ds1)) == (N0 (Just ds2)) = ds1 `eq` ds2
--     where
--       eq = (==) `F.on` dropLeadZero
-- 
-- instance Eq N' where
--   (N' ds1) == (N' ds2) = ds1 == ds2
-- 
-- -- NaN is defined as positive infinitity
-- instance Ord N0 where
--   (N0 Nothing) `compare` (N0 Nothing) = EQ
--   (N0 Nothing) `compare` _ = GT
--   (N0 (Just _)) `compare` (N0 Nothing) = LT
--   (N0 (Just ds1)) `compare` (N0 (Just ds2)) = ds1 `comp` ds2
--     where
--       comp = compareDs `F.on` dropLeadZero
-- 
-- instance Ord N' where
--   (N' ds1) `compare` (N' ds2) = (NE.toList ds1) `compare` (NE.toList ds2)
-- 
-- instance Num N0 where
--   (N0 n1) + (N0 n2) = N0 $ addDlist <$> n1 <*> n2
--   (N0 n1) * (N0 n2) = N0 $ mulDlist <$> n1 <*> n2
--   abs = id
--   signum n
--     | n == (N0 (Just [D0])) = n
--     | otherwise = N0 (Just [D1])
--   fromInteger i 
--     | i == 0 = N0 (Just [D0])
--     | i > 0 = N0 $ (dropLeadZero . reverse) <$> (mkDRlistFromInt i)
--     | otherwise = N0 Nothing
--   negate n
--     | n == (N0 (Just [D0])) = n
--     | otherwise = N0 Nothing
-- 
-- mkN' :: [D] -> N'
-- mkN' xs = case dropLeadZero xs of 
--   [] -> N' $ D0 NE.:| []
--   xs -> N' $ NE.fromList xs
-- 
-- nZero' :: N'
-- nZero' = N' $ D0 NE.:| []
-- 
-- addN' :: N' -> N' -> N'
-- addN' (N' ns) (N' ms) = mkN' $ reverse $ addDRlist ns' ms'
--   where
--     ns' = NE.toList $ NE.reverse ns
--     ms' = NE.toList $ NE.reverse ms


---------------------- Natural number -------------------------



--mkN :: String -> Maybe N
--mkN cs = (N . dropLeadZero) <$> (mapM fromChar cs)

instance HumanReadable N where
  fromString s = case dropLead ' ' $ dropTrail ' ' s of
    [] -> Nothing
    cs -> (N. dropLeadZero) <$> (mapM fromChar cs)

  toString n = case getDs n of 
    [] -> "0"
    ds -> fmap toChar ds


  --(N ds1) == (N ds2) = ds1 `eq` ds2
  --  where
  --    eq = (==) `F.on` getDs

  --(N ds1) `compare` (N ds2) = ds1 `comp` ds2
  --  where
  --    comp = compareDs `F.on` dropLeadZero

-- instance Num N where
--   (+) = addN
--   (*) = mulN
--   abs = id
--   signum n
--     | n == nZero = nZero
--     | otherwise = nOne
--   fromInteger i 
--     | i == 0 = nZero
--     | i > 0 = case mkDRlistFromInt i of
--       Just n -> mkN (reverse n)
--       Nothing -> nZero
--     | otherwise = undefined
--   negate = undefined

-- instance Real N where
--   toRational = nToInteger

-- instance Integral N where
--   -- since it is for natural numbers only, quotRem and divMod are identical
--   n1 `quotRem` n2 = case n1 `divModN` n2 of
--     Nothing -> error "divide by zero for type N"
--     Just ret -> ret
--   n1 `divMod` n2 = n1 `quotRem` n2 -- otherwise it depends on negate function from Num class
--   toInteger = nToInteger



--------------------- number based utility functions ---------------------
replicateN :: N -> a -> [a]
replicateN n x
  | n == nZero = []
  | otherwise = x:(replicateN (subN n nOne) x)
          
chunkN :: N -> [a] -> ([a], [a], N)
chunkN n xs | n == nZero = ([], xs, nZero)
chunkN n [] = ([], [], nZero)
chunkN n (x:xs) = (x:front, back, subN n' nOne)
  where
    (front, back, n') = chunkN (subN n nOne) xs -- n > 0 here

---------------------------- Integer Number -----------------------------
data Z 
  = Z 
  { getZAbs :: N 
  , getSign :: Bool -- True means positive, 0 can be either way 
  } deriving Show

mkZ :: N -> Bool -> Z
mkZ n s
  | n == nZero = Z nZero True
  | otherwise = Z n s

instance HumanReadable Z where
  fromString s = case dropLead ' ' $ dropTrail ' ' s of 
    ('-':cs) -> mkZ <$> fromString cs <*> pure False
    cs -> mkZ <$> fromString cs <*> pure True

  toString z = if getSign z then zabs else "-" ++ zabs
    where 
      zabs = toString (getZAbs z)

zZero :: Z
zZero = mkZ nZero True

zOne :: Z
zOne = mkZ nOne True

zMinusOne :: Z
zMinusOne = mkZ nOne False

invertOrd :: Ordering -> Ordering
invertOrd GT = LT
invertOrd LT = GT
invertOrd EQ = EQ

instance Enum Z where
  fromEnum z = let val = fromEnum (getZAbs z) 
    in if getSign z then val else negate val
  toEnum d
    | d == 0 = zZero
    | d > 0 = mkZ (toEnum d) True
    | otherwise = mkZ (toEnum (-d)) False

instance Eq Z where
  z1 == z2 = (getZAbs z1) == (getZAbs z2) && (getSign z1) == (getSign z2)

instance Ord Z where
  z1 `compare` z2 
    | n1 == nZero && n2 == nZero = EQ
    | n1 /= nZero && n2 == nZero = if s1 then GT else LT
    | n1 == nZero && n2 /= nZero = if s2 then LT else GT
    | s1 && s2 = n1 `compare` n2
    | s1 && not s2 = GT
    | not s1 && not s2 = n2 `compare` n1
    | otherwise = LT -- not s1 && s2
    where 
      n1 = getZAbs z1
      n2 = getZAbs z2
      s1 = getSign z1
      s2 = getSign z2

addZ :: Z -> Z -> Z
addZ z1 z2 
  | z1 == zZero = z2
  | z2 == zZero = z1
  | s1 && s2 = mkZ (addN n1 n2) True
  | not s1 && not s2 = mkZ (addN n1 n2) False
  | s1 && not s2 = if n1 > n2 then mkZ (subN n1 n2) True else mkZ (subN n2 n1) False
  | otherwise = if n2 > n1 then mkZ (subN n2 n1) False else mkZ (subN n1 n2) True -- not s1 && s2
  where 
    n1 = getZAbs z1
    n2 = getZAbs z2
    s1 = getSign z1
    s2 = getSign z2

mulZ :: Z -> Z -> Z
mulZ z1 z2 = mkZ zabs sign
  where 
    zabs = mulN (getZAbs z1) (getZAbs z2)
    sign = (getSign z1) && (getSign z2)

instance Num Z where
  (+) = addZ
  (*) = mulZ
  abs z = mkZ (getZAbs z) True
  signum i
    | i == 0 = zZero
    | i > 0 = zOne
    | otherwise = zMinusOne 
  fromInteger i
    | i == 0 = zZero
    | i > 0 = case nFromInteger i of 
      Just n -> mkZ n True
      _ -> zZero -- shouldn't happen
    | otherwise = negate $ fromInteger $ negate i
  negate z = mkZ (getZAbs z) (not $ getSign z)

instance Real Z where
  toRational z = (if getSign z then id else negate) $ nToInteger (getZAbs z)

--quotRemZ :: Z -> Z -> (Z, Z)
--quotRemZ i1@(Z n1 s1) i2@(Z n2 s2)
--  | i2 == iZero = error "divide by zero for type Z"
--  | i1 == iZero = (iZero, iZero)
--  | s1 && s2 = (posZFromN q, posZFromN r)
--  | s1 && not s2 = (negZFromN q, posZFromN r)
--  | not s1 && s2 = (negZFromN q, negZFromN r)
--  | otherwise = (posZFromN q, negZFromN r) -- not s1 && not s2
--  where 
--    (q, r) = n1 `quotRem` n2

--instance Integral Z where
--  quotRem = quotRemZ
--  toInteger (Z n s)
--    | n >= nZero = toInteger n
--    | otherwise = negate $ toInteger n


---------------- Decimal fraction representation -----------------------
data DF
  = DF
  { getZ :: Z
  , getDecimalPlaces :: N
  }

simplifyDF :: DF -> DF
simplifyDF (DF (Z (N ds) s) dec) = let (ds', dec') = run ((reverse ds), dec)
  in DF (Z (N $ reverse ds') s) dec'
  where 
    run :: ([D], N) -> ([D], N)
    run ([], _) = ([D0], 0)
    run ([D0], _) = ([D0], 0)
    run ((D0:ds), n)
      | n > 0 = run (ds, (subN n 1))
    run (ds, n) = (ds, n)

constructDF :: Z -> N -> DF
constructDF z n = simplifyDF (DF z n)

instance Eq DF where
  df1@(DF z1 dec1) == df2@(DF z2 dec2) = z1 == z2 && dec1 == dec2

instance Ord DF where
  df1@(DF z1 dec1) `compare` df2@(DF z2 dec2) = z1' `compare` z2'
    where 
      dec' = max dec1 dec2
      z1' = iterateN (subN dec' dec1) z1 (*10)
      z2' = iterateN (subN dec' dec2) z2 (*10)

instance HumanReadable DF where
  fromString cs = case S.splitOn "." $ dropLead ' ' $ dropTrail ' ' cs of
    [] -> Nothing
    [s] -> DF <$> fromString s <*> pure (N [D0])
    [s, d] -> let
        d' = dropTrail '0' d
        dec = ((toInteger . length) <$> (mapM fromChar d')) >>= nFromInteger
        z = fromString (s ++ d')
      in DF <$> z <*> dec
    _ -> Nothing -- incorrect format with too many dots

  toString (DF (Z n s) dec)
    | n == nZero = "0"
    | s = display n dec
    | otherwise = "-" ++ display n dec
    where
      display :: N -> N -> String
      display n dec
        | dCount > dec = let (front, back, _) = chunkN (subN dCount dec) ds
          in fmap toChar front ++ "." ++ fmap toChar back
        | otherwise = "0." ++ (fmap toChar $ replicateN (subN dec dCount) D0) ++ show n
        where 
          dCount = nDigitCount n
          ds = getDs n

isInteger :: DF -> Bool
isInteger = (== nZero) . getDecimalPlaces

intPart :: DF -> Z
intPart (DF z@(Z n@(N ds) s) dec)
  | dec == nZero = z
  | n == nZero = 0
  | otherwise = Z (N ds') s
  where
    ds' = (reverse . drop (fromInteger $ toInteger dec) . reverse) $ ds

mulByTen :: DF -> DF
mulByTen (DF z dec) 
  | dec == 0 = DF (z*10) dec
  | otherwise = DF z (subN dec nOne)

divByTen :: DF -> DF
divByTen (DF z@(Z n s) dec)
  | dec > nZero = DF z (dec+nOne)
  | otherwise = case reverse $ getDs n of
    D0:rest -> DF (Z (N $ reverse rest) s) nZero
    ds' -> DF z nOne

iterateN :: N -> a -> (a -> a) -> a
iterateN n x f = last $ take n' $ iterate f x
  where
    n' = fromInteger $ toInteger $ n+nOne

addDF :: DF -> DF -> DF
addDF (DF z1 dec1) (DF z2 dec2) = simplifyDF $ DF (z1'+z2') dec'
  where 
    dec' = max dec1 dec2
    z1' = iterateN (subN dec' dec1) z1 (*10)
    z2' = iterateN (subN dec' dec2) z2 (*10)

mulDF :: DF -> DF -> DF
mulDF (DF z1 dec1) (DF z2 dec2) = simplifyDF $ DF (z1*z2) (dec1+dec2)

instance Num DF where
  (+) = addDF
  (*) = mulDF
  abs (DF z dec) = simplifyDF $ DF (abs z) dec
  signum (DF z _) = simplifyDF $ DF (signum z) nZero
  fromInteger i = simplifyDF $ DF (fromInteger i) nZero
  negate (DF z dec) = simplifyDF $ DF (negate z) dec

--------------------------- Fraction ----------------------------------
data F = F N N Bool -- numerator, denominator and sign, denom > 0

debugF :: F -> String
debugF (F (N n) (N d) s) = "F:" ++ show n ++ "/" ++ show d ++ "," ++ show s

getDenom :: F -> N
getDenom (F _ d _) = d

simplify :: (N, N) -> (N, N)
simplify (num, denom)
  | denom == nZero = (num, denom)
  | num == nZero = (nZero, nOne) -- without this, 0/5 causes issue in otherwise
  | divisor == nOne = (num, denom)
  | otherwise = ((num `quot` divisor), (denom `quot` divisor))
  where
    divisor = gcdN num denom

simplifyF :: F -> F
simplifyF (F n d s) = let (n', d') = simplify (n, d) in F n' d' s

constrcutMabyeF :: N -> N -> Bool -> Maybe F
constrcutMabyeF n1 n2 s
  | n2' == nZero = Nothing
  | otherwise = Just $ F n1' n2' s
  where
    (n1', n2') = simplify (n1, n2)

constructF :: N -> N -> Bool -> F
constructF n1 n2 s
  | n2 == 0 = constructF n1 1 s
  | otherwise = let (n1', n2') = simplify (n1, n2) in F n1' n2' s

instance HumanReadable F where
  fromString ('-':cs) = negate <$> fromString cs
  fromString cs = case rest of
    [] -> fFromZ <$> (Z <$> num <*> pure True)
    ('/':cs') -> F <$> num <*> fromString cs' <*> pure True
    _ -> Nothing 
    where
      num = fromString $ takeWhile (/= '/') cs
      rest = dropWhile (/= '/') cs

  toString (F n1 n2 s)
    | n2 == nZero = error "divide by zero in type F"
    | n2 == nOne = toString (Z n1 s)
    | otherwise = if s then frac else "-" ++ frac
    where 
      frac = toString n1 ++ "/" ++ toString n2


posFFromNs :: N -> N -> Maybe F
posFFromNs n1 n2 = constrcutMabyeF n1 n2 True

negFFromNs :: N -> N -> Maybe F
negFFromNs n1 n2  = constrcutMabyeF n1 n2 False

fFromZ :: Z -> F
fFromZ (Z n s) = F n nOne s

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
    | n1' == nZero || n2' == nZero = False
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
      then simplifyF $ F (subN num1 num2) (d1'*d2') True
      else simplifyF $ F (subN num2 num1) (d1'*d2') False
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


------------------------ expression in algebra -------------------------
-- this also covers type Z and F
--data E
--  = NaN 
--  | IntNum Z
--  | Frac E E
--  | Exp E E
--
--paren s = "(" ++ s ++ ")"
--
--instance Show E where
--  show NaN = "NaN"
--  show (IntNum z) = show z
--  show (Frac a b) = paren (show a) ++ "/" ++ paren (show b) -- need improvement
--  show (Exp b e) = paren (show b) ++ "^" ++ paren (show e)
--
--instance Num E where
--  NaN + _ = NaN
--  _ + NaN = NaN
--  (IntNum z1) + (IntNum z2) = IntNum (z1 + z2)
--   + _ = undefined
--
--  (*) = undefined
--  abs = undefined
--  signum = undefined
--  fromInteger = undefined
--  negate = undefined
