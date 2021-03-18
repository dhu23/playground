import Data.List (intercalate)

data Triangle = Triangle { up     :: Int
                         , height :: Int }
              deriving (Show, Eq)

fill :: Int -> Int -> Int -> [String]
fill h w s = let fillLn n = replicate (s-n) '_'   ++
                            replicate (1+2*n) '1' ++
                            replicate (w-s-n-1) '_'
              in map fillLn [0..h-1]

sierpinski :: Int -> Int -> [String]
sierpinski hgt n =
    let initt :: Int -> Int -> Int -> Int -> Int -> [String]
        initt h w s 0 _ = fill h w s
        initt h w s l d =
            let lvl1   = initt (div h 2) w s (l-1) (d+1)
                lvl2_1 = initt (div h 2) s (s-1-div w (2^(d+1))) (l-1) d
                lvl2_2 = initt (div h 2) (w-s-1) (div w (2^(d+1))) (l-1) d
                lvl2   = zipWith (++) lvl2_1 $ zipWith (++) (replicate (div h 2) "_") lvl2_2
             in lvl1 ++ lvl2
     in initt hgt (2*hgt-1) (hgt-1) n 1

draw :: [String] -> IO ()
draw mat = putStrLn $ intercalate "\n" mat

main :: IO ()
main = do
    n <- readLn :: IO Int
    draw $ sierpinski 32 n
