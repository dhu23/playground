module QC.TestNatural
  ( checkNatural
  ) where

import Test.QuickCheck
import Arithmetic.Natural
  ( N
  , SumN(..)
  , ProdN(..)
  , fromStr
  , toStr
  , n'0
  , n'1
  , mkN
  , addN
  , subN
  , mulN
  , divModN
  , nToInteger
  , nFromInteger
  )
import QC.ArithmeticArbitrary
  ( AnyN(..)
  , SmallN(..)
  , prop_monoidUnit
  , prop_commutativity
  , prop_associativity
  )


prop_readable :: N -> Bool
prop_readable n = case fromStr $ toStr n of
  Nothing -> False
  Just n' -> n' == n


prop_NFromInt :: Int -> Bool
prop_NFromInt i = fromEnum n == absi
  where
    absi = abs i
    n = toEnum absi :: N


-- use AnyN can cause overflow issue
prop_NEnum :: SmallN -> Bool
prop_NEnum (SmallN n) = n' == n
  where 
    n' = toEnum (fromEnum n) :: N


prop_NIntegerConv :: N -> Bool
prop_NIntegerConv n = case nFromInteger (nToInteger n) of
  Nothing -> False
  Just n' -> n == n'


prop_subUnit :: N -> Bool
prop_subUnit n = n `subN` n'0 == n


prop_sub :: N -> N -> Bool
prop_sub nx ny = test1 && test2
  where
    s = nx `addN` ny
    test1 = s `subN` nx == ny
    test2 = s `subN` ny == nx


prop_mulByZero :: N -> Bool
prop_mulByZero n = n'0 == n `mulN` n'0


prop_divByZero :: N -> Bool
prop_divByZero n = Nothing == n `divModN` n'0

prop_divByOne :: N -> Bool
prop_divByOne n = case n `divModN` n'1 of
  Just (q, r) -> q == n && r == n'0
  Nothing -> False

prop_divMod :: N -> N -> Bool
prop_divMod nx ny
  | ny == n'0 = prop_divByZero nx
  | ny == n'1 = prop_divByOne nx
  | otherwise = case nx `divModN` ny of
    Nothing -> False
    Just (q, r) -> r < ny && (nx == (ny `mulN` q) `addN` r)

prop_compare :: N -> N -> Bool
prop_compare nx ny
  | nx > ny = nx `subN` ny > n'0
  | nx < ny = ny `subN` nx > n'0
  | otherwise = nx `subN` ny == n'0


checkNatural :: IO ()
checkNatural = do
  putStrLn "testing Arithmetic.Natural"
  quickCheck prop_readable'
  quickCheck prop_NFromInt
  quickCheck prop_NEnum
  quickCheck prop_NIntegerConv'
  quickCheck prop_addUnit
  quickCheck prop_mulUnit
  quickCheck prop_addComm
  quickCheck prop_mulComm
  quickCheck prop_addAssoc
  quickCheck prop_mulAssoc
  quickCheck prop_subUnit'
  quickCheck prop_sub'
  quickCheck prop_mulByZero'
  quickCheck prop_divByZero'
  quickCheck prop_divByOne'
  quickCheck prop_compare'
  where
    prop_readable' (AnyN n) = prop_readable n
    prop_NIntegerConv' (AnyN n) = prop_NIntegerConv n
    prop_addUnit (AnyN n) = prop_monoidUnit (SumN n)
    prop_mulUnit (AnyN n) = prop_monoidUnit (ProdN n)
    prop_addComm (AnyN x) (AnyN y) = prop_commutativity (SumN x) (SumN y)
    prop_mulComm (AnyN x) (AnyN y) = prop_commutativity (ProdN x) (ProdN y)
    prop_addAssoc (AnyN x) (AnyN y) (AnyN z) = prop_associativity (SumN x) (SumN y) (SumN z)
    prop_mulAssoc (AnyN x) (AnyN y) (AnyN z) = prop_associativity (ProdN x) (ProdN x) (ProdN z)
    prop_subUnit' (AnyN n) = prop_subUnit n
    prop_sub' (AnyN x) (AnyN y) = prop_sub x y
    prop_mulByZero' (AnyN n) = prop_mulByZero n
    prop_divByZero' (AnyN n) = prop_divByZero n
    prop_divByOne' (AnyN n) = prop_divByOne n
    prop_compare' (AnyN x) (AnyN y) = prop_compare x y
    prop_divMod' (AnyN x) (AnyN y) = prop_divMod x y

