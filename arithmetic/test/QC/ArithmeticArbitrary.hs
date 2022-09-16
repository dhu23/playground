module QC.ArithmeticArbitrary
  ( AnyDigit(..)
  , toDs
  --, AnyNatural
  ) where

import Test.QuickCheck
import Arithmetic.Digit (D)

newtype AnyDigit 
  = AnyDigit 
  { getD :: D
  } deriving (Eq, Show)


toDs :: [AnyDigit] -> [D]
toDs = fmap getD


instance Arbitrary AnyDigit where
  arbitrary = do
    d <- elements [minBound .. maxBound]
    return $ AnyDigit d

