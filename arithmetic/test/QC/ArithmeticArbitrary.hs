module QC.ArithmeticArbitrary
  ( AnyD(..)
  , AnyN(..)
  , SmallN(..)
  , AnyZ(..)
  , prop_monoidUnit
  , prop_commutativity
  , prop_associativity
  ) where

import Test.QuickCheck
import Debug.Trace
import Arithmetic.Digit (D)
import Arithmetic.Natural (N, mkN)
import Arithmetic.Integer (Z, mkZ)
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


newtype AnyZ 
  = AnyZ
  { getZ :: Z
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


instance Arbitrary AnyZ where
  arbitrary = do
    (AnyN n) <- arbitrary
    sign <- arbitrary
    return $ AnyZ $ mkZ n sign


prop_monoidUnit :: (Eq m, Monoid m) => m -> Bool
prop_monoidUnit x = test1 && test2
  where
    test1 = (x <> mempty) == x
    test2 = (mempty <> x) == x
 

prop_commutativity :: (Eq m, Semigroup m) => m -> m -> Bool
prop_commutativity x y = (x <> y) == (y <> x)


prop_associativity :: (Eq m, Monoid m) => m -> m -> m ->  Bool
prop_associativity x y z = (x <> y) <> z == x <> (y <> z)

