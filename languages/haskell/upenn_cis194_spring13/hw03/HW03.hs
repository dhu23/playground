import qualified Data.Map as DM
import qualified Data.List as DL

--exercise 1 Hopscotch
--
--skips :: [a] -> [[a]]
--
--skips "ABCD" == ["ABCD", "BD", "C", "D"]
--the first list in the output should be the same as the input 
--the second should contain every second element from the input 
--the nth shoudl contain every nth element in the input 

skipN :: [a] -> Int -> [a]
skipN xs n = map fst $ filter isNth $ zip xs [1..]
    where 
        isNth (x, i) = i `mod` n == 0

skips :: [a] -> [[a]]
skips xs = map (skipN xs) [1..length xs]

--exercise 2 local maxima
--a local maximum of a list is an elment of the list which is 
--strictly greater than both the neighbors.
thd (x, y, z) = z

localMaxima :: [Integer] -> [Integer]
localMaxima ns 
    | length ns < 3 = []
    | otherwise = map thd $ filter localMax $ zip3 fronts backs mids
    where
        fronts = init $ init ns
        backs = tail $ tail ns
        mids = init $ tail ns
        localMax (f, b, m) = m > f && m > b


localMaxima' :: [Integer] -> [Integer]
localMaxima' (a:b:ns) = reverse $ thd $ foldl run ([a], b, []) ns
    where 
        run (ns'@(n':_), mid, ret) n
            | mid > n && mid > n' = ((mid:ns'), n, (mid:ret))
            | otherwise = ((mid:ns'), n, ret)


--exercise 3 Histogram
--histogram [1, 1, 1, 5] == 
--
-- *
-- *
-- *   *
-- ========
--0123456789 

counts :: [Integer] -> DM.Map Integer Int
counts ns = foldr run z ns
    where
        z = DM.fromList $ zip [0..9] $ (repeat 0) 
        run n m = DM.adjust (+1) n m

getM :: Ord k => DM.Map k a -> k -> (a -> Bool) -> b -> b -> b
getM m k f v0 v = case DM.lookup k m of 
                      Nothing -> v0
                      Just c -> if f c then v0 else v

makeLine :: DM.Map Integer Int -> (String, DM.Map Integer Int)
makeLine m = (map getChar [0..9], foldr countDown m [0..9])
    where 
        getChar k = getM m k (<=0) ' ' '*' 
        countDown k m = DM.adjust (1 `subtract`) k m


stillValid :: DM.Map Integer Int -> Bool
stillValid m = not $ all (==False) $ map isGoodCount [0..9]
    where
        isGoodCount k = getM m k (<=0) False True

makeHistogram :: DM.Map Integer Int -> [String]
makeHistogram m
    | stillValid m = let (l, m') = makeLine m in (makeHistogram m') ++ [l]
    | otherwise = []
                     

histogram :: [Integer] -> String
histogram = DL.intercalate "\n" . addAxis . makeHistogram . counts
    where 
        addAxis hist = hist ++ [replicate 10 '='] ++ 
                       [take 10 $ iterate succ '0']

main = do
    print $ skips "ABCD"
    print $ skips "hello!"
    print $ skips [1]
    print $ skips [True, False]
    print $ skips ([] :: [Int])
    print $ localMaxima' [2, 9, 5, 6, 1]
    print $ localMaxima' [2, 3, 4, 1, 5]
    print $ localMaxima' [1, 2, 3, 4, 5]
    print $ counts [1, 1, 1, 5]
    putStrLn $ histogram [1, 1, 1, 5]
    --print $ counts [1, 4, 5, 4, 6, 6, 3, 4, 2, 4, 9]
    --print $ makeLine $ counts [1, 4, 5, 4, 6, 6, 3, 4, 2, 4, 9]
    --print $ makeLine $ (snd . makeLine) $ counts [1, 4, 5, 4, 6, 6, 3, 4, 2, 4, 9]
    putStrLn $ histogram [1, 4, 5, 4, 6, 6, 3, 4, 2, 4, 9]
    
