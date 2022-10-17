fibRecursive :: Int -> Int
fibRecursive 0 = 0
fibRecursive 1 = 1
fibRecursive n = fibRecursive (n-1) + fibRecursive (n-2)



main :: IO ()
main = do
  print $ sum $ map fibRecursive [1..39]
