module QC.TestDigit 
  ( checkDigit
  ) where

import Test.QuickCheck
import Debug.Trace
import qualified Data.Bifunctor as Bf (bimap)

import Arithmetic.Digit
  ( D(..)
  , fromChar
  , toChar
  , fromStr
  , toStr
  , addD
  , subD
  , mulD
  )
import QC.ArithmeticArbitrary (AnyD(..))


prop_readable :: D -> Bool
prop_readable d = case fromStr $ toStr d of
  Nothing -> False
  Just d' -> d' == d


prop_DCharConversion :: D -> Bool
prop_DCharConversion d = d' == Just d
  where
    d' = fromChar (toChar d)


prop_addD :: D -> D -> Bool
prop_addD dx dy = (fromEnum dx) + (fromEnum dy) == (tupleToInt $ addD dx dy)


prop_subD :: D -> D -> Bool
prop_subD dx dy = (if negative then 10 + diff else diff) == fromEnum d
  where
    (negative, d) = subD dx dy
    diff = (fromEnum dx) - (fromEnum dy)


prop_mulD :: D -> D -> Bool
prop_mulD dx dy = (fromEnum dx) * (fromEnum dy) == (tupleToInt $ mulD dx dy)


tupleToInt :: (D, D) -> Int
tupleToInt (ten, single) = (fromEnum ten) * 10 + (fromEnum single)


checkDigit :: IO ()
checkDigit = do
  putStrLn "testing Arithmetic.Digit"
  quickCheck prop_readable'
  quickCheck prop_DCharConversion'
  quickCheck prop_addD'
  quickCheck prop_subD'
  quickCheck prop_mulD'
  where
    prop_readable' (AnyD d) = prop_readable d
    prop_DCharConversion' (AnyD d) = prop_DCharConversion d
    prop_addD' (AnyD dx) (AnyD dy) = prop_addD dx dy
    prop_subD' (AnyD dx) (AnyD dy) = prop_subD dx dy
    prop_mulD' (AnyD dx) (AnyD dy) = prop_mulD dx dy
