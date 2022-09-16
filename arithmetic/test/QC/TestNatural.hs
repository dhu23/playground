module QC.TestNatural
  ( checkNatural
  ) where

import Test.QuickCheck
import Arithmetic.Natural
  ( N
  , mkN
  )
import QC.ArithmeticArbitrary
  ( AnyN(..)
  , SmallN(..)
  )


prop_NEnum1 :: Int -> Bool
prop_NEnum1 i 
  | i < 0 = True -- no need to test it
  | otherwise = fromEnum n == i 
  where
    n = toEnum i :: N


-- use AnyN can cause overflow issue
prop_NEnum2 :: SmallN -> Bool
prop_NEnum2 (SmallN n) = n' == n
  where 
    n' = toEnum (fromEnum n) :: N


checkNatural :: IO ()
checkNatural = do
  putStrLn "testing Arithmetic.Natural"
  quickCheck prop_NEnum1
  quickCheck prop_NEnum2

