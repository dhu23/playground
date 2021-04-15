import Control.Monad (forM_)

-- very slow for large n. exponential complexity
catalan' :: Integer -> Integer
catalan' 0 = 1
catalan' 1 = 1
catalan' n = sum $ map term [0..n-1]
    where term i = (catalan' i) * (catalan' (n-1-i)) 

-- very fast 
catalan :: Integer -> Integer 
catalan 0 = 1
catalan 1 = 1
catalan n = head cache
    where cache = foldl step [1, 1] [2..n]
          step acc i = let next = sum $ zipWith (*) acc (reverse acc)          
                       in next:acc

main :: IO ()
main = do
    --forM_ [0..20] $ \i -> print $ catalan' i 
    forM_ [0..20] $ \i -> print $ catalan i
