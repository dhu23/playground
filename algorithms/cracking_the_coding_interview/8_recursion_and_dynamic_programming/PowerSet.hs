import Data.List (sort, inits)
import Control.Monad (forM_)

powerSet :: [Int] -> [[Int]]
powerSet [] = [[]]
powerSet (x:xs) = set ++ (map (sort . (x:)) $ set)
    where set = powerSet xs

-- powerSet can be viewed as a sequence of "yes" "no" "yes" "yes" 
-- for each element of the original array of size n
-- now the problem is to generate a sequence of 1 (yes) and 0 (no) with size n
allSeq :: Int -> [[Int]]
allSeq 0 = [[]]
allSeq 1 = [[1], [0]]
allSeq n = (map (1:) sub) ++ (map (0:) sub)
    where sub = allSeq (n-1)

-- hornet's algorithm
toInt :: Int -> [Int] -> Int
toInt base xs = foldl step 0 xs
    where step acc i = acc*base + i

main :: IO ()
main = do
    forM_ (map powerSet $ inits [1..5]) $ \i -> do print ((length i), i)
    forM_ (allSeq 5) $ \i -> do print (i, toInt 2 i)
