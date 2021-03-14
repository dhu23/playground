-- Kadane's algorithm to solve the max subarray sum

maxSubarraySum :: [Integer] -> Integer
maxSubarraySum xs = fst $ foldl step (0, 0) xs
    where 
        step (maxSum, currSum) x
            | x >= 0 = ((max maxSum currSum'), currSum')
            | currSum' < maxSum = (maxSum, 0)
            | otherwise = (currSum', currSum')
            where currSum' = currSum + x

maxSubarraySum2 :: [Integer] -> [Integer]
maxSubarraySum2 xs = foldl step [] xs
    where 
        step [] x = [x]
        step cs@(currMax:cms) x = (max (currMax + x) 0):cs


maxSubarraySum3 :: [Integer] -> [Integer]
maxSubarraySum3 xs = zipWith (-) cumSums cumMins 
    where 
        cumSums = scanl (+) 0 xs
        cumMins = scanl min 0 cumSums

main :: IO ()
main = do
    let inputs = [1, -2, 3, 4, -1, 1] :: [Integer]
    print $ maxSubarraySum inputs
    print $ maxSubarraySum2 inputs
    print $ maxSubarraySum3 inputs
