import Control.Monad (mapM)
import qualified Data.List.NonEmpty as NE

data D
  = D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8 | D9 
  deriving (Enum, Eq, Ord, Show)


fromChar :: Char -> Maybe D
fromChar '0' = Just D0
fromChar '1' = Just D1
fromChar '2' = Just D2
fromChar '3' = Just D3
fromChar '4' = Just D4
fromChar '5' = Just D5
fromChar '6' = Just D6
fromChar '7' = Just D7
fromChar '8' = Just D8
fromChar '9' = Just D9
fromChar _ = Nothing


toChar :: D -> Char
toChar D0 = '0'
toChar D1 = '1'
toChar D2 = '2'
toChar D3 = '3'
toChar D4 = '4'
toChar D5 = '5'
toChar D6 = '6'
toChar D7 = '7'
toChar D8 = '8'
toChar D9 = '9'

toInt :: D -> Int
toInt D0 = 0
toInt D1 = 1
toInt D2 = 2
toInt D3 = 3
toInt D4 = 4
toInt D5 = 5
toInt D6 = 6
toInt D7 = 7
toInt D8 = 8
toInt D9 = 9

dSeq = iterate succ D0

-- maybe I can just map the results exhaustively
addD :: D -> D -> (D, D)
addD d1 d2
  | q == 0 = (D0, dSeq !! r)
  | otherwise = (D1, dSeq !! r)
  where
    (q, r) = ((toInt d1) + (toInt d2)) `divMod` 10


-- the most significant digit comes first. We could use a NonEmpty list
-- but it is not necessary. Because it has to do certain validation to trim 
-- down length like "01234" to store as "1234", it might as well not allow
-- empty list []
-- the value constructor should not be exported
data NInt = NInt [D] 


instance Show NInt where
  show (NInt ds) = fmap toChar ds


instance Eq NInt where
  n1 == n2 = undefined


mkNInt :: String -> Maybe NInt
mkNInt cs = fmap (NInt . dropLeadZero) $ mapM fromChar cs >>= NE.nonEmpty
  where
    dropLeadZero = NE.dropWhile (== D0)


-- least significant digit comes first
runAdd' :: [D] -> [D] -> [D]
runAdd' [] ys = ys
runAdd' xs [] = xs -- xs is not []
runAdd' (x:xs) (y:ys) = case tens of 
  D0 -> single:s'
  otherwise -> runAdd' [single, tens] (D0:s')
  where
    (tens, single) = addD x y
    s' = runAdd' xs ys



addNInt :: NInt -> NInt -> NInt
addNInt (NInt ns) (NInt ms) = NInt $ runAdd ns' ms'
  where
    ns' = reverse ns
    ms' = reverse ms

    runAdd ns ms = case runAdd' ns ms of
      [] -> [D0]
      ret -> reverse ret