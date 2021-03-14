import Control.Monad (forM_, mapM_, foldM)

printList :: Show a => [a] -> IO ()
printList = mapM_ print

-- generalize the bunny invasion in the list monad for an arbitrary
-- number of generations


generation = replicate 3

xs `withGenerations` n 
    | n == 0 = xs
    | otherwise = (xs >>= generation) `withGenerations` (n-1)
