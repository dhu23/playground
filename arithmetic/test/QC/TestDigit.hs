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


prop_DCharConversion :: D -> Bool
prop_DCharConversion d = d' == Just d
  where
    d' = fromChar (toChar d)


prop_DlistConversion :: Int -> Bool
prop_DlistConversion i
  | i < 0 = True
  | otherwise = ((fmap toChar) <$> ds) == Just (show i)
  where
    ds = mkDlistFromInt i


prop_addUnit :: [D] -> Bool
prop_addUnit ds = test1 && test2
  where 
    test1 = [] `addDlist` ds == ds
    test2 = ds `addDlist` [] == ds


prop_addCommutativeLaw' :: [D] -> [D] -> Bool
prop_addCommutativeLaw' dxs dys = s1 == s2
  where
    s1 = dropLeadZero $ dxs `addDlist` dys
    s2 = dropLeadZero $ dys `addDlist` dxs


prop_sub :: [D] -> [D] -> Bool
prop_sub dxs dys = test1 && test2
  where
    s = dxs `addDlist` dys
    test1 = dropLeadZero (s `subDlist` dxs) == dropLeadZero dys
    test2 = dropLeadZero (s `subDlist` dys) == dropLeadZero dxs


prop_mulUnit :: [D] -> Bool
prop_mulUnit ds = test1 && test2
  where
    test1 = dropLeadZero (ds `mulDlist` [D1]) == dropLeadZero ds
    test2 = dropLeadZero ([D1] `mulDlist` ds) == dropLeadZero ds


prop_mulCommutativeLaw' :: [D] -> [D] -> Bool
prop_mulCommutativeLaw' dxs dys = p1 == p2
  where
    p1 = dropLeadZero $ dxs `mulDlist` dys
    p2 = dropLeadZero $ dys `mulDlist` dxs


prop_divByZero :: [D] -> Bool
prop_divByZero ds = Nothing == ds `divModDlist` []


prop_divByOne :: [D] -> Bool
prop_divByOne ds = expected == ret
  where
    expected = Just (dropLeadZero ds, [])
    ret = Bf.bimap dropLeadZero dropLeadZero <$> ds `divModDlist` [D1]


prop_divMod :: [D] -> [D] -> Bool
prop_divMod dxs dys
  | dys `compareDs` [] == EQ = prop_divByZero dxs
  | dys `compareDs` [D1] == EQ = prop_divByOne dxs
  | otherwise = test1 && test2
  where
    restruct denom (q, r) = dropLeadZero $ (denom `mulDlist` q) `addDlist` r
    ret = dxs `divModDlist` dys
    restructed = restruct dys <$> ret
    satisfy denom (_, r) = denom `compareDs` r /= LT
    test1 = Just (dropLeadZero dxs) == restructed
    test2 = Just True == (satisfy dys <$> ret)


prop_compare :: [D] -> [D] -> Bool
prop_compare dxs dys
  | compRes == EQ = dropLeadZero (dxs `subDlist` dys) == []
  | compRes == GT = (dxs `subDlist` dys) `compareDs` [] == GT
  | otherwise = (dys `subDlist` dxs) `compareDs` [] == GT
  where 
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


instance Arbitrary D where
  arbitrary = elements [minBound .. maxBound]
