import Test.QuickCheck
import Arithmetic

instance Arbitrary D where
  arbitrary = elements [minBound .. maxBound]

instance Arbitrary N where
  arbitrary = undefined

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