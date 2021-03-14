data Rate = SimpleRate Double
          | ContinuouslyCompoundedRate Double
          | NFoldCompoundedRate Int Double
          deriving (Show, Eq)

rateReturn :: Double -> Double -> Rate -> Double
rateReturn start end (SimpleRate l)
    | end >= start = 1 + (end-start)*l
    | otherwise = error "bad"
rateReturn start end (ContinuouslyCompoundedRate y)
    | end >= start = exp (y*(end-start))
    | otherwise = error "bad"
rateReturn start end (NFoldCompoundedRate n y)
    | end >= start = (1+y/n')**(n'*(end-start))
    | otherwise = error "bad"
    where n' = fromIntegral n

discountFactor start end = (1.0/) . (rateReturn start end)


test1 :: IO ()
test1 = do 
    print $ rateReturn 0 1 (SimpleRate 0.05)
    print $ rateReturn 0 1 (ContinuouslyCompoundedRate 0.04)
    print $ rateReturn 0 1 (NFoldCompoundedRate 12 0.04)
    print $ discountFactor 0 1 (SimpleRate 0.05)
    print $ discountFactor 0 1 (ContinuouslyCompoundedRate 0.04)

main :: IO ()
main = do
    test1
