module Arithmetic.Integer
  ( Z
  , mkZ
  , posZ
  , negZ
  , SumZ(..)
  , ProdZ(..)
  , z'0
  , pz'1
  , nz'1
  , zToInteger
  , zFromInteger
  ) where


import Arithmetic.Natural
  ( N
  , n'0
  , n'1
  , mkN
  , addN
  , subN
  , mulN
  , divModN
  , quotRemN
  , nToInteger
  , nFromInteger
  )

data Z
  = Z 
  { zAbs :: N
  , zSign :: Bool -- True means positive, 0 is constructed with True
  } deriving Show

mkZ :: N -> Bool -> Z
mkZ n s
  | n == n'0 = z'0
  | otherwise = Z n s

posZ :: N -> Z
posZ n = Z n True

negZ :: N -> Z
negZ n
  | n == n'0 = posZ n
  | otherwise = Z n False


z'0 = posZ n'0
pz'1 = posZ n'1
nz'1 = negZ n'1

instance Eq Z where
  (Z nx sx) == (Z ny sy) = nx == ny && sx == sy

instance Enum Z where
  fromEnum (Z n s) = (fromEnum n) * (if s then 1 else -1)
  toEnum i
    | i >= 0 = posZ $ toEnum i
    | otherwise = negZ $ toEnum (negate i)

instance Ord Z where
  zx `compare` zy
    | zx == z'0 && zy == z'0 = EQ
    | zx == z'0 && zy /= z'0 = if zSign zy then LT else GT
    | zy == z'0 = if zSign zx then GT else LT
    | zx > z'0 && zy < z'0 = GT
    | zx < z'0 && zy > z'0 = LT
    | zSign zx = (zAbs zx) `compare` (zAbs zy)
    | otherwise = (zAbs zy) `compare` (zAbs zx)


newtype SumZ = SumZ Z deriving (Eq, Show)

instance Semigroup SumZ where
  (SumZ x) <> (SumZ y) = SumZ $ x `addZ` y

-- instance Semigroup SumZ where
instance Monoid SumZ where
  mempty = SumZ z'0

newtype ProdZ = ProdZ Z deriving (Eq, Show)

instance Semigroup ProdZ where
  (ProdZ x) <> (ProdZ y) = ProdZ $ x `mulZ` y

instance Monoid ProdZ where
  mempty = ProdZ pz'1

instance Num Z where
  (+) = addZ
  (*) = mulZ
  abs = posZ . zAbs
  signum z
    | z > z'0 = pz'1
    | z < z'0 = nz'1
    | otherwise = z'0
  negate z@(Z n s)
    | z == z'0 = z'0
    | otherwise = Z n (not s)
  fromInteger = zFromInteger


zFromInteger :: Integer -> Z
zFromInteger i
  | i == 0 = z'0
  | i > 0 = case nFromInteger i of
    Nothing -> undefined -- shouldn't happen
    Just n -> posZ n
  | otherwise = negate $ zFromInteger (-i)


zToInteger :: Z -> Integer
zToInteger (Z n s) = (nToInteger n) * (if s then 1 else -1)


addZ :: Z -> Z -> Z
addZ zx@(Z nx _) zy@(Z ny _)
  | zx == z'0 = zy
  | zy == z'0 = zx
  | zx > z'0 && zy > z'0 = posZ $ nx `addN` ny
  | zx < z'0 && zy < z'0 = negZ $ nx `addN` ny
  | zx > z'0 && zy < z'0 = if nx >= ny 
    then posZ $ nx `subN` ny 
    else negZ $ ny `subN` nx
  | otherwise = if nx >= ny
    then negZ $ nx `subN` ny
    else posZ $ ny `subN` nx

mulZ :: Z -> Z -> Z
mulZ (Z nx sx) (Z ny sy) = mkZ (nx `mulN` ny) (sx && sy)
    
