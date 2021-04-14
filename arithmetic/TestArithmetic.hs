import Test.QuickCheck
import Arithmetic
import qualified Control.Monad as M (replicateM)
import qualified Data.List as L (sort)

import Debug.Trace
-- trace is a very useful function to debug
-- say I want to see how what happens inside of prop_NEnum
-- I can use 
-- prop_NEnum' i = trace ("passing integer:" + show i) (prop_NEnum i)

--------------------  based off integer conversion -----------------------
prop_DCharConversion :: D -> Bool
prop_DCharConversion d = d' == Just d
  where
    d' = fromChar (toChar d) 

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


checkThroughConversions :: IO ()
checkThroughConversions = do
  quickCheck prop_DCharConversion
  quickCheck prop_NEnum
  quickCheck prop_NShow'
  quickCheck prop_IEnum
  quickCheck prop_IDivMod
  quickCheck prop_IQuotRem

  
-- ---------------- innate mathematical property based ----------------------
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
      then return $ posIFromN n
      else return $ negIFromN n

instance Arbitrary F where
  arbitrary = do
    num <- arbitrary
    denom <- arbitrary
    sign <- arbitrary
    let denom' = max denom 1
    return $ constructF num denom' sign
      

prop_PosDenom :: F -> Bool
prop_PosDenom f = (getDenom f) > 0

prop_NDivMod0 :: N -> Bool
prop_NDivMod0 n = n `divModN` nZero == Nothing

prop_NDivMod1 :: N -> Bool
prop_NDivMod1 n = n `divModN` nOne == Just (n, nZero)

prop_NDivMod :: N -> N -> Bool
prop_NDivMod n1 n2 = case n1 `divModN` n2 of
  Nothing -> n2 == nZero
  Just (d, m) -> m < n2 && d * n2 + m == n1

prop_addUnit :: (Eq a, Num a) => a -> Bool
prop_addUnit i = i + 0 == i

prop_subUnit :: (Eq a, Num a) => a -> Bool
prop_subUnit i = i - 0 == i

prop_mulUnit :: (Eq a, Num a) => a -> Bool
prop_mulUnit i = i * 1 == i

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

prop_addSubDuality0 :: (Eq a, Show a, Num a) => a -> a -> Bool
prop_addSubDuality0 i1 i2 = i1 - i2 == i1 + (negate i2)
        
prop_compareSub :: (Eq a, Ord a, Num a) => a -> a -> Bool
prop_compareSub i1 i2
  | i1 == i2 = diff == 0
  | i1 > i2 = diff > 0
  | otherwise = diff < 0
  where 
    diff = i1 - i2

checkNLaws :: IO ()
checkNLaws = do
  quickCheck prop_NDivMod0
  quickCheck prop_NDivMod1
  quickCheck prop_NDivMod
  quickCheck (prop_addUnit :: N -> Bool)
  quickCheck (prop_mulUnit :: N -> Bool)
  quickCheck (prop_addCommutativeLaw :: N -> N -> Bool)
  quickCheck (prop_mulCommutativeLaw  :: N -> N -> Bool)
  quickCheck (prop_addAssociativeLaw :: N -> N -> N -> Bool)
  quickCheck (prop_mulAssociativeLaw :: N -> N -> N -> Bool)
  quickCheck (prop_mulDistributiveLaw :: N -> N -> N -> Bool)


checkILaws :: IO ()
checkILaws = do
  quickCheck (prop_addUnit :: I -> Bool)
  quickCheck (prop_subUnit :: I -> Bool)
  quickCheck (prop_mulUnit :: I -> Bool)
  quickCheck (prop_addSubDuality0 :: I -> I -> Bool)
  quickCheck (prop_compareSub :: I -> I -> Bool)
  quickCheck (prop_addCommutativeLaw :: I -> I -> Bool)
  quickCheck (prop_mulCommutativeLaw  :: I -> I -> Bool)
  quickCheck (prop_addAssociativeLaw :: I -> I -> I -> Bool)
  quickCheck (prop_mulAssociativeLaw :: I -> I -> I -> Bool)
  quickCheck (prop_mulDistributiveLaw :: I -> I -> I -> Bool)


checkFLaws :: IO ()
checkFLaws = do
  quickCheck prop_PosDenom
  quickCheck (prop_addUnit :: F -> Bool)
  quickCheck (prop_subUnit :: F -> Bool)
  quickCheck (prop_mulUnit :: F -> Bool)
  quickCheck (prop_addSubDuality0 :: F -> F -> Bool)
  quickCheck (prop_compareSub :: F -> F -> Bool)
  quickCheck (prop_addCommutativeLaw :: F -> F -> Bool)
  quickCheck (prop_mulCommutativeLaw  :: F -> F -> Bool)
  quickCheck (prop_addAssociativeLaw :: F -> F -> F -> Bool)
  quickCheck (prop_mulAssociativeLaw :: F -> F -> F -> Bool)
  quickCheck (prop_mulDistributiveLaw :: F -> F -> F -> Bool)


-- I noticed that running this compiled is way much faster than in ghci
main = do
  trace "checking conversion..." checkThroughConversions
  trace "checking for N..." checkNLaws
  trace "checking for I..." checkILaws
  trace "checking for F..." checkFLaws
