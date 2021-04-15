import qualified Data.Set as S
import qualified Data.Map as M

permutationNoDup :: [a] -> [[a]]
permutationNoDup [] = [[]]
permutationNoDup [a] = [[a]]
permutationNoDup (a:as) = concat $ map splitInsert $ permutationNoDup as
    where insert x (list1, list2) = list1++[x]++list2
          splitInsert xs = map (insert a) $ 
                           map (flip splitAt xs) [0..length xs]

countMap :: Ord k => [k] -> M.Map k Integer
countMap = foldr step M.empty
    where step x m = case M.member x m of 
                        True -> M.adjust (+1) x m
                        False -> M.insert x 1 m

-- M.fold (a->a->b)->b->Map k a->b
{-
permutationWithDup :: Ord a => [a] -> [[a]]
permutationWithDup xs = perm cm [] (length xs) []
    where cm = countMap xs
          positiveKeys = M.foldrWithKey step []
          step k v pos | v > 0 = k:pos
                       | otherwise = pos
          perm m prefix remain ret
             | remain == 0 = prefix:ret
             | otherwise = 
                    let positives = positiveKeys m 
                        modMaps = map (\x -> M.adjust (-1) x m) positives
                        ret' = concat $ 
                               map (\(c, m') -> perm m' (c:prefix) (remain-1)) 
                               $ zip positives $ modMaps
                    in ret++ret'
-}

-- an "un-functional" way of doing it. inspired by an imperative solution
swap :: [a] -> Int -> Int -> [a]
swap xs i j
    | i < 0 || i >= strLen || j < 0 || j >= strLen = error "Bad index"
    | i == j = xs
    | i < j = let left = take i xs
                  right = drop (j+1) xs
                  mid = drop (i+1) $ take j xs
              in left ++ [xs !! j] ++ mid ++ [xs !! i] ++ right
    | otherwise = swap xs j i
    where strLen = length xs

permutationNoDup' :: Int -> [a] -> [[a]]
permutationNoDup' n xs
    | n+1 == strLen = [xs]
    | n < strLen = concat $ map (permutationNoDup' (n+1)) $ swapedStrs
    where strLen = length xs
          swapedStrs = map (swap xs n) [n..strLen-1]

permutationWithDup' :: Ord a => Int -> [a] -> [[a]]
permutationWithDup' n xs 
    | n+1 == strLen = [xs]
    | n < strLen = concat $ map (permutationWithDup' (n+1)) $ swapedStrs
    where strLen = length xs
          -- the different of this from permutationNoDup' is that 
          -- in this case, char duplicates in [n..strLen-1] range is removed
          swapedStrs = map (swap xs n) filtered
          filtered = snd $ foldr step (S.empty, []) [n..strLen-1]
          step i (s, indices) = let ele = xs!!i 
                                in case S.member ele s of
                                       True -> (s, indices)
                                       False -> (S.insert ele s, i:indices)


main :: IO ()
main = do
    print $ permutationNoDup "abcd"
    print $ permutationNoDup "abcde"
    print $ permutationNoDup' 0 "abc"
    print $ permutationNoDup' 0 "abcde"
    print $ permutationWithDup' 0 "aaabaaaa" -- only 8 variations
