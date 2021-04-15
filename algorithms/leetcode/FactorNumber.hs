-- factor num using the first at least as large as factor
-- and the quotient should be larger than the first factor too
-- f(num, factor) -> f1 * f2, where f2 >= f1 >= factor
factorIt :: Int -> Int -> [Int]
factorIt num factor 
    | factor2 > num = []
    | otherwise = let ret = filter ((==0).snd.fst) $
                            takeWhile ((>=factor).fst.fst) $ 
                            zip (map (divMod num) [factor..]) [factor..]
                  in case ret of
                      [] -> []
                      ((q,r),f):xs -> [q, f]
    where factor2 = factor*factor

type Factors = [Int]

-- for factoring 24:
-- 1 * [3, 2, 2, 2] with 2 should yield 1 * [4, 3, 2] with 3
-- in details 6 * [2, 2] with 3 -> 12 * [2] with 3


main :: IO()
main = do 
    putStrLn $ show $ factorIt 24 5
    putStrLn $ show $ waysOfFactoring 24
