import Test.QuickCheck
import Arithmetic
import qualified Control.Monad as M (replicateM)

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
