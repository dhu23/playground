module QC.ArithmeticArbitrary
  ( AnyD(..)
  , AnyN(..)
  , SmallN(..)
  , toDs
  ) where

import Test.QuickCheck
import Arithmetic.Digit (D)
import Arithmetic.Natural (N, mkN)
import qualified Control.Monad as CM

newtype AnyD 
  = AnyD 
  { getD :: D
  } deriving (Eq, Show)


newtype AnyN
  = AnyN
  { getN :: N
  } deriving (Eq, Show)


newtype SmallN
  = SmallN
  { getSN :: N
  } deriving (Eq, Show)


toDs :: [AnyD] -> [D]
toDs = fmap getD


-- this would be an orphan instance, a warning depending on setting
-- We use newtypes to wrap them
-- instance Arbitrary D where
--   arbitrary = elements [minBound .. maxBound]


instance Arbitrary AnyD where
  arbitrary = do
    d <- elements [minBound .. maxBound]
    return $ AnyD d


instance Arbitrary AnyN where
  arbitrary = do
    anyds <- arbitrary
    return $ AnyN $ mkN $ toDs anyds


instance Arbitrary SmallN where
  arbitrary = do
    l <- choose (0, 15)
    anyds <- CM.replicateM l arbitrary
    return $ SmallN $ mkN $ toDs anyds

    
