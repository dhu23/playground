module QC.TestInteger
  ( checkInteger
  ) where


import Test.QuickCheck
import Debug.Trace
import Arithmetic.Integer
  ( Z
  , posZ
  , negZ
  , SumZ(..)
  , ProdZ(..)
  , zToInteger
  , zFromInteger
  )
import QC.ArithmeticArbitrary
  ( AnyZ(..)
  , prop_monoidUnit
  , prop_commutativity
  , prop_associativity
  )

prop_ZIntegerConv :: Z -> Bool
prop_ZIntegerConv z = z == zFromInteger (zToInteger z)

prop_numOrd :: (Ord m, Num m) => m -> m -> Bool
prop_numOrd x y
  | x > y = x - y > x - x
  | x < y = x - y < x - x
  | otherwise = x - y == x - x


checkInteger :: IO ()
checkInteger = do
  putStrLn "testing Arithmetic.Integer"
  quickCheck prop_ZIntegerConv'
  quickCheck prop_addUnit
  quickCheck prop_mulUnit
  quickCheck prop_addComm
  quickCheck prop_mulComm
  quickCheck prop_addAssoc
  quickCheck prop_mulAssoc
  quickCheck prop_numOrd'
  where
    prop_ZIntegerConv' (AnyZ z) = prop_ZIntegerConv z
    prop_addUnit (AnyZ z) = prop_monoidUnit (SumZ z)
    prop_mulUnit (AnyZ z) = prop_monoidUnit (ProdZ z)
    prop_addComm (AnyZ x) (AnyZ y) = prop_commutativity (SumZ x) (SumZ y)
    prop_mulComm (AnyZ x) (AnyZ y) = prop_commutativity (ProdZ x) (ProdZ y)
    prop_addAssoc (AnyZ x) (AnyZ y) (AnyZ z) = prop_associativity (SumZ x) (SumZ y) (SumZ z)
    prop_mulAssoc (AnyZ x) (AnyZ y) (AnyZ z) = prop_associativity (ProdZ x) (ProdZ y) (ProdZ z)
    prop_numOrd' (AnyZ x) (AnyZ y) = prop_numOrd x y
  
