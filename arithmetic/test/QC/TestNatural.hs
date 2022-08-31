module QC.TestNatural
  ( checkNatural
  ) where

import Test.QuickCheck

import Arithmetic.Natural
  ( N
  )


prop_NEnum :: Int -> Bool
prop_NEnum i 
  | i < 0 = True -- no need to test it
  | otherwise = fromEnum n == i 
  where
    n = toEnum i :: N


checkNatural :: IO ()
checkNatural = do
  putStrLn "testing Arithmetic.Natural"
  quickCheck prop_NEnum
