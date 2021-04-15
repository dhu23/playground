interweave :: [a] -> [a] -> [[a]]
interweave x [] = [x]
interweave [] y = [y]
interweave x@(x0:xs) y@(y0:ys) = 
    let first = map ([x0]++) $ interweave xs y
        second = map ([y0]++) $ interweave x ys                           
    in first ++ second


main :: IO()
main = do
    putStrLn $ show $ interweave [1, 2] [3, 4]
    putStrLn $ show $ interweave [1, 2, 3] [4, 5, 6]
