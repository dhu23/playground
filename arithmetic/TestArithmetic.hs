import Test.QuickCheck
import Arithmetic

instance Arbitrary D where
  arbitrary = elements [minBound .. maxBound]

prop_DCharConversion :: D -> Bool
prop_DCharConversion d = d' == Just d
  where
    d' = fromChar (toChar d) 
