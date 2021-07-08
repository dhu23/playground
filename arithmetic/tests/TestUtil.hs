module TestUtil where

import Test.QuickCheck
import Arithmetic.Number.Symbolic

instance Arbitrary D where
  arbitrary = elements [minBound .. maxBound]
