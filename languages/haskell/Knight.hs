withinBoard :: Int -> Int -> Bool
withinBoard x y = (withinRange x) && (withinRange y)

withinRange x = (x >= 0) && (x < 8)

plusMinus x d = [(x+d), (x-d)]

chancesWithin :: Int -> Int -> Int -> Double
chancesWithin m x y
  | not (withinBoard x y) = 0.0
  | m == 0 = 1.0
  | otherwise = 0.125 *
      ( sum (chancesWithin (m-1) <$> (x `plusMinus` 2) <*> (y `plusMinus` 1))
      + sum (chancesWithin (m-1) <$> (x `plusMinus` 1) <*> (y `plusMinus` 2))
      )

chancesWithin' m x y 
  | not (withinBoard x y) = 0.0
  | m == 0 = 1.0
  | otherwise = 0.125 *
      ( chancesWithin' (m-1) (x+2) (y+1) 
      + chancesWithin' (m-1) (x+2) (y-1)
      + chancesWithin' (m-1) (x-2) (y+1)
      + chancesWithin' (m-1) (x-2) (y-1)
      + chancesWithin' (m-1) (x+1) (y+2)
      + chancesWithin' (m-1) (x+1) (y-2)
      + chancesWithin' (m-1) (x-1) (y+2)
      + chancesWithin' (m-1) (x-1) (y-2)
      )
