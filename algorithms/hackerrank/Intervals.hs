type Interval a = (a, a)

intersect :: Ord a => Interval a -> Interval a -> Bool
intersect (a1, b1) (a2, b2) 
    | b1 < a2 || a1 > b2 = False
    | otherwise = True

mergeInterval :: [Interval Int] -> Interval Int -> [Interval Int]
mergeInterval [] x = [x]
mergeInterval (p@(pa, pb):ps) x@(xa, xb)
    | xb < pa = x:p:ps
    | intersect p x = mergeInterval ps (min pa xa, max pb xb)
    | otherwise = p : (mergeInterval ps x)

merge :: [Interval Int] -> [Interval Int]
merge = foldl mergeInterval []

intervals :: [Interval Int]
intervals = [(1, 3), (4, 9), (2, 15), (14, 20), (12, 13), (25, 30)]


main = do 
    print $ merge intervals   
    print $ merge [(1, 20), (12, 13)]
    print $ intersect (1, 20) (12, 13)
