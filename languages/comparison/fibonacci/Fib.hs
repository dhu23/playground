fibRecursive :: Int -> Int
fibRecursive 0 = 0
fibRecursive 1 = 1
fibRecursive n = fibRecursive (n-1) + fibRecursive (n-2)


timeTest :: Int -> Int
timeTest v = sum $ map fibRecursive [1..v]


main :: IO ()
main = do
  print $ timeTest 43
