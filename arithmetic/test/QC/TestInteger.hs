module QC.TestInteger
  ( checkInteger
  ) where


import Test.QuickCheck
import Debug.Trace
import Arithmetic.Integer
  ( Z
  , fromStr
  , toStr
  , posZ
  , negZ
  , z'0
  , pz'1
  , SumZ(..)
  , ProdZ(..)
  , quotRemZ
  )
import QC.ArithmeticArbitrary
  ( AnyZ(..)
  , prop_monoidUnit
  , prop_commutativity
  , prop_associativity
  )


prop_readable :: Z -> Bool
prop_readable z = case fromStr $ toStr z of
  Nothing -> False
  Just z' -> z == z'

prop_ZIntegerConv :: Z -> Bool
prop_ZIntegerConv z = z == fromInteger (toInteger z)

prop_numOrd :: (Ord m, Num m) => m -> m -> Bool
prop_numOrd x y
  | x > y = x - y > x - x
  | x < y = x - y < x - x
  | otherwise = x - y == x - x

prop_divByZero :: Z -> Bool
prop_divByZero z = Nothing == z `quotRemZ` z'0

prop_divByOne :: Z -> Bool
prop_divByOne z = case z `quotRemZ` pz'1 of
  Nothing -> False
  Just (q, r) -> q == z && r == z'0

prop_quotRemZ :: Z -> Z -> Bool
prop_quotRemZ zx zy
  | zy == z'0 = prop_divByZero zx
  | zy == pz'1 = prop_divByOne zx
  | otherwise = case zx `quotRemZ` zy of 
    Nothing -> False
    Just (q, r) -> let 
      test1 = ((abs r) < (abs zy))
      test2 = q*zy + r == zx
      ix = toInteger zx
      iy = toInteger zy
      (iq, ir) = ix `quotRem` iy
      test3 = (toInteger q) == iq
      test4 = (toInteger r) == ir
      in test1 && test2 && test3 && test4


checkInteger :: IO ()
checkInteger = do
  putStrLn "testing Arithmetic.Integer"
  quickCheck prop_readable'
  quickCheck prop_ZIntegerConv'
  quickCheck prop_addUnit
  quickCheck prop_mulUnit
  quickCheck prop_addComm
  quickCheck prop_mulComm
  quickCheck prop_addAssoc
  quickCheck prop_mulAssoc
  quickCheck prop_numOrd'
  quickCheck prop_divByZero'
  quickCheck prop_divByOne'
  quickCheck prop_quotRemZ'
  where
    prop_readable' (AnyZ z) = prop_readable z
    prop_ZIntegerConv' (AnyZ z) = prop_ZIntegerConv z
    prop_addUnit (AnyZ z) = prop_monoidUnit (SumZ z)
    prop_mulUnit (AnyZ z) = prop_monoidUnit (ProdZ z)
    prop_addComm (AnyZ x) (AnyZ y) = prop_commutativity (SumZ x) (SumZ y)
    prop_mulComm (AnyZ x) (AnyZ y) = prop_commutativity (ProdZ x) (ProdZ y)
    prop_addAssoc (AnyZ x) (AnyZ y) (AnyZ z) = prop_associativity (SumZ x) (SumZ y) (SumZ z)
    prop_mulAssoc (AnyZ x) (AnyZ y) (AnyZ z) = prop_associativity (ProdZ x) (ProdZ y) (ProdZ z)
    prop_numOrd' (AnyZ x) (AnyZ y) = prop_numOrd x y
    prop_divByZero' (AnyZ z) = prop_divByZero z
    prop_divByOne' (AnyZ z) = prop_divByOne z
    prop_quotRemZ' (AnyZ x) (AnyZ y) = prop_quotRemZ x y
