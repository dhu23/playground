import Test.QuickCheck
import TestUtil
import qualified Arithmetic.Number.Symbolic as S

prop_DCharConversion :: D -> Bool
prop_DCharConversion d = fromChar (toChar d) == Just d


main = do
  quickCheck prop_DCharConversion
