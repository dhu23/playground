module QC.TestDigit 
  ( checkDigit
  ) where

import Test.QuickCheck
import Debug.Trace
import qualified Data.Bifunctor as Bf (bimap)

import Arithmetic.Digit
  ( D(D1)
  , fromChar
  , toChar
  , mkDlistFromInt
  , addDlist
  , subDlist
  , mulDlist
  , divModDlist
  , compareDs
  , dropLeadZero
  )
import QC.ArithmeticArbitrary 
  ( AnyDigit(..)
  , toDs
  )


prop_DCharConversion :: AnyDigit -> Bool
prop_DCharConversion (AnyDigit d) = d' == Just d
  where
    d' = fromChar (toChar d)


prop_DlistConversion :: Int -> Bool
prop_DlistConversion i
  | i < 0 = True
  | otherwise = ((fmap toChar) <$> ds) == Just (show i)
  where
    ds = mkDlistFromInt i


prop_addUnit :: [AnyDigit] -> Bool
prop_addUnit anyds = test1 && test2
  where 
    ds = toDs anyds
    test1 = [] `addDlist` ds == ds
    test2 = ds `addDlist` [] == ds


prop_addCommutativeLaw' :: [AnyDigit] -> [AnyDigit] -> Bool
prop_addCommutativeLaw' anydxs anydys = s1 == s2
  where
    dxs = toDs anydxs
    dys = toDs anydys
    s1 = dropLeadZero $ dxs `addDlist` dys
    s2 = dropLeadZero $ dys `addDlist` dxs


prop_sub :: [AnyDigit] -> [AnyDigit] -> Bool
prop_sub anydxs anydys = test1 && test2
  where
    dxs = toDs anydxs
    dys = toDs anydys
    s = dxs `addDlist` dys
    test1 = dropLeadZero (s `subDlist` dxs) == dropLeadZero dys
    test2 = dropLeadZero (s `subDlist` dys) == dropLeadZero dxs


prop_mulUnit :: [AnyDigit] -> Bool
prop_mulUnit anyds = test1 && test2
  where
    ds = toDs anyds
    test1 = dropLeadZero (ds `mulDlist` [D1]) == dropLeadZero ds
    test2 = dropLeadZero ([D1] `mulDlist` ds) == dropLeadZero ds


prop_mulCommutativeLaw' :: [AnyDigit] -> [AnyDigit] -> Bool
prop_mulCommutativeLaw' anydxs anydys = p1 == p2
  where
    dxs = toDs anydxs
    dys = toDs anydys
    p1 = dropLeadZero $ dxs `mulDlist` dys
    p2 = dropLeadZero $ dys `mulDlist` dxs


prop_divByZero :: [AnyDigit] -> Bool
prop_divByZero anyds = let ds = toDs anyds in Nothing == ds `divModDlist` []


prop_divByOne :: [AnyDigit] -> Bool
prop_divByOne anyds = expected == ret
  where
    ds = toDs anyds
    expected = Just (dropLeadZero ds, [])
    ret = Bf.bimap dropLeadZero dropLeadZero <$> ds `divModDlist` [D1]


prop_divMod :: [AnyDigit] -> [AnyDigit] -> Bool
prop_divMod anydxs anydys
  | dys `compareDs` [] == EQ = prop_divByZero anydxs
  | dys `compareDs` [D1] == EQ = prop_divByOne anydxs
  | otherwise = test1 && test2
  where
    dxs = toDs anydxs
    dys = toDs anydys
    restruct denom (q, r) = dropLeadZero $ (denom `mulDlist` q) `addDlist` r
    ret = dxs `divModDlist` dys
    restructed = restruct dys <$> ret
    satisfy denom (_, r) = denom `compareDs` r /= LT
    test1 = Just (dropLeadZero dxs) == restructed
    test2 = Just True == (satisfy dys <$> ret)


prop_compare :: [AnyDigit] -> [AnyDigit] -> Bool
prop_compare anydxs anydys
  | compRes == EQ = dropLeadZero (dxs `subDlist` dys) == []
  | compRes == GT = (dxs `subDlist` dys) `compareDs` [] == GT
  | otherwise = (dys `subDlist` dxs) `compareDs` [] == GT
  where 
    dxs = toDs anydxs
    dys = toDs anydys
    compRes = dxs `compareDs` dys


checkDigit :: IO ()
checkDigit = do
  putStrLn "testing Arithmetic.Digit"
  quickCheck prop_DCharConversion
  quickCheck prop_DlistConversion
  quickCheck prop_addUnit
  quickCheck prop_addCommutativeLaw'
  quickCheck prop_sub
  quickCheck prop_mulUnit
  quickCheck prop_mulCommutativeLaw'
  quickCheck prop_divByZero
  quickCheck prop_divByOne
  quickCheck prop_divMod
  quickCheck prop_compare
