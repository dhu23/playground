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

prop_IQuotRem :: Integer -> Integer -> Bool
prop_IQuotRem i1 i2
  | i2 == 0 = True
  | otherwise = (i1 `quotRem` i2) == (toInteger q, toInteger r)
  where
    (q, r) = (fromInteger i1) `quotRem` (fromInteger i2)

prop_IDivMod :: Integer -> Integer -> Bool
prop_IDivMod i1 i2
  | i2 == 0 = True
  | otherwise = (i1 `divMod` i2) == (toInteger q, toInteger r)
  where
    (q, r) = (fromInteger i1) `divMod` (fromInteger i2)
  
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

instance Arbitrary F where
  arbitrary = do 
    num <- arbitrary
    denom <- arbitrary
    sign <- arbitrary
    let f = if sign then posF else negF
    case f num denom of
      Nothing -> arbitrary
      Just ret -> return ret

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

prop_addCommutativeLaw :: (Eq a, Num a) => a -> a -> Bool
prop_addCommutativeLaw x y = x + y == y + x

prop_mulCommutativeLaw :: (Eq a, Num a) => a -> a -> Bool
prop_mulCommutativeLaw x y = x * y == y * x

prop_addAssociativeLaw :: (Eq a, Num a) => a -> a -> a -> Bool
prop_addAssociativeLaw x y z = (x + y) + z == x + (y + z)

prop_mulAssociativeLaw :: (Eq a, Num a) => a -> a -> a -> Bool
prop_mulAssociativeLaw x y z = (x * y) * z == x * (y * z)

prop_mulDistributiveLaw :: (Eq a, Num a) => a -> a -> a -> Bool
prop_mulDistributiveLaw x y z = x * (y + z) == x * y + x * z 

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
  --quickCheck prop_NDivMod
  quickCheck prop_IAddUnit
  quickCheck prop_ISubUnit
  quickCheck prop_IMulUnit
  quickCheck (prop_addCommutativeLaw :: N -> N -> Bool)
  quickCheck (prop_addCommutativeLaw :: I -> I -> Bool)
  --quickCheck (prop_addCommutativeLaw :: F -> F -> Bool)
  quickCheck (prop_mulCommutativeLaw  :: N -> N -> Bool)
  quickCheck (prop_mulCommutativeLaw  :: I -> I -> Bool)
  --quickCheck (prop_mulCommutativeLaw  :: N -> N -> Bool)
  quickCheck (prop_addAssociativeLaw :: N -> N -> N -> Bool)
  quickCheck (prop_addAssociativeLaw :: I -> I -> I -> Bool)
  --quickCheck (prop_addAssociativeLaw :: N -> N -> Bool)
  quickCheck (prop_mulAssociativeLaw :: N -> N -> N -> Bool)
  quickCheck (prop_mulAssociativeLaw :: I -> I -> I -> Bool)
  --quickCheck (prop_mulAssociativeLaw :: N -> N -> Bool)
  quickCheck (prop_mulDistributiveLaw :: N -> N -> N -> Bool)
  quickCheck (prop_mulDistributiveLaw :: I -> I -> I -> Bool)
  --quickCheck (prop_mulDistributiveLaw :: N -> N -> Bool)
  quickCheck prop_ISubAddDuality
  quickCheck prop_ICompareSub