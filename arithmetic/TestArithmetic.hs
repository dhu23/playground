import Test.QuickCheck
import Arithmetic
import qualified Control.Monad as M (replicateM)
import qualified Data.List as L (sort)

-- based off integer conversion
prop_NEnum :: Int -> Bool
prop_NEnum i
  | i < 0 = True -- no need to test it
  | otherwise = fromEnum n == i
  where 
    n = toEnum i :: N

prop_NShow' :: Integer -> Bool
prop_NShow' i
  | i < 0 = True -- no need to test it
  | otherwise = show n == show i
  where 
    n = toEnum $ fromIntegral i :: N

prop_IEnum :: Int -> Bool
prop_IEnum i = fromEnum i' == i
  where
    i' = toEnum i :: I

-- innate property based
instance Arbitrary D where
  arbitrary = elements [minBound .. maxBound]

instance Arbitrary N where
  arbitrary = do 
    l <- getSize
    ds <- M.replicateM l arbitrary
    return $ nFromList ds

instance Arbitrary I where
  arbitrary = do
    n <- arbitrary
    sign <- arbitrary
    if sign 
      then return $ posI n
      else return $ negI n

prop_DCharConversion :: D -> Bool
prop_DCharConversion d = d' == Just d
  where
    d' = fromChar (toChar d) 

prop_NDivMod0 :: N -> Bool
prop_NDivMod0 n = n `divModN` nZero == Nothing

prop_NDivMod1 :: N -> Bool
prop_NDivMod1 n = n `divModN` nOne == Just (n, nZero)

prop_NDivMod :: N -> N -> N -> Bool
prop_NDivMod n1 n2 n3 = case0 && case1 && case2
  where
    (minn:midn:maxn:[]) = L.sort [n1, n2, n3]
    case0 = 
      let v = (maxn * midn + minn) `divModN` maxn
      in 
        if maxn > nZero 
          then v == Just (midn, minn)
          else v == Nothing
    case1 = 
      let v = (maxn * midn + minn) `divModN` midn
      in 
        if midn > nZero 
          then v == Just (maxn, minn)
          else v == Nothing
    case2 = 
      let v = (maxn * minn + midn) `divModN` maxn
      in 
        if maxn > nZero 
          then v == Just (minn, midn) 
          else v == Nothing

prop_IAddUnit :: I -> Bool
prop_IAddUnit i = i + iZero == i

prop_ISubUnit :: I -> Bool
prop_ISubUnit i = i - iZero == i

prop_IMulUnit :: I -> Bool
prop_IMulUnit i = i * iOne == i

prop_IAddCommutativeLaw :: I -> I -> Bool
prop_IAddCommutativeLaw i1 i2 = i1 + i2 == i2 + i1

prop_IMulCommutativeLaw :: I -> I -> Bool
prop_IMulCommutativeLaw i1 i2 = i1 * i2 == i2 * i1

prop_IAddAssociativeLaw :: I -> I -> I -> Bool
prop_IAddAssociativeLaw i1 i2 i3 = (i1 + i2) + i3 == i1 + (i2 + i3)

prop_IMulAssociativeLaw :: I -> I -> I -> Bool
prop_IMulAssociativeLaw i1 i2 i3 = (i1 * i2) * i3 == i1 * (i2 * i3)

prop_IMulDistributiveLaw :: I -> I -> I -> Bool
prop_IMulDistributiveLaw i1 i2 i3 = i1 * (i2 + i3) == i1 * i2 + i1 * i3

prop_ISubAddDuality :: I -> I -> Bool
prop_ISubAddDuality i1 i2 = i1 - i2 == i1 + (negate i2)

prop_ICompareSub :: I -> I -> Bool
prop_ICompareSub i1 i2
  | i1 == i2 = diff == iZero
  | i1 > i2 = diff > iZero
  | otherwise = diff < iZero
  where 
    diff = i1 - i2

-- I noticed that running this compiled is much faster than in ghci
main = do
  quickCheck prop_NEnum
  quickCheck prop_NShow'
  quickCheck prop_IEnum
  quickCheck prop_DCharConversion
  quickCheck prop_NDivMod0
  quickCheck prop_NDivMod
  quickCheck prop_IAddUnit
  quickCheck prop_ISubUnit
  quickCheck prop_IMulUnit
  quickCheck prop_IAddCommutativeLaw
  quickCheck prop_IMulCommutativeLaw
  quickCheck prop_IAddAssociativeLaw
  quickCheck prop_IMulAssociativeLaw
  quickCheck prop_IMulDistributiveLaw
  quickCheck prop_ISubAddDuality
  quickCheck prop_ICompareSub
  
