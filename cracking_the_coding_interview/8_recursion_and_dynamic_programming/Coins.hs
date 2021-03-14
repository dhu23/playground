import Data.List (sortBy)
import qualified Data.Map as M 

--coinCombos :: [Int] -> [[Int]]
--coinCombos = init . tails

-- given amount n and coins, return how many ways of representing it
-- the table is in the format of (n, i) -> count#
{-
countCoins :: Int -> [Int] -> Int
countCoins n coins = undefined
    where combos = coinCombos coins
          coinCount = length coins
          initTable = M.fromList $ zip (zip [0,0..] [0..coinCount-1]) [1,1..]
          table = foldl calcTable initTable [1..n]
            where calcTable oldTable i = foldr calcRow oldTable pairs
                where pairs = zip [i,i..] [0..coinCount-1]
                      cv = head $ combos !! j
                      calcRow (n,j) t
                        | j == coinCount-1 = M.insert 
                                             (n,j) 
                                             (if n `mod` cv == 0 then 1 else 0)
                                             t
                        | n < cv = M.insert (n, j) table
-}

countCoins :: Int -> [Int] -> Int
countCoins 0 _ = 1 -- 1 way of doing it: choose nothing
countCoins n [] | n > 0 = 0
countCoins n denoms = 
    (\i -> 0) $ foldr step M.empty $ [(amt, upTo) | amt <- [0..n], upTo <- [0..coinL-1]]
    where coinL = length denoms
          step (amt, fromIdx) m
            | fromIdx >= coinL = m -- folding stops for invalid input
            | calculated = m
            | amt < denoms !! fromIdx = step (amt, fromIdx+1) m
            | otherwise = let ways = sum $ map build choices
                          in M.insert (amt, fromIdx) ways m
            where calculated = M.member (amt, fromIdx) m
                  denom = denoms !! fromIdx
                  valid i = i*denom <= amt
                  build = (flip step) (fromIdx+1) . (amt-) . (*denom)
                  choices = takeWhile valid [0..]
main = do 
    print $ countCoins 100 [1, 5, 10, 25]
