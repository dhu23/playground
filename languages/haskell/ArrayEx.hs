import Data.Array
import Data.List (groupBy, intercalate)
import Data.Function (on)

hist :: (Ix a, Integral b) => (a, a) -> [a] -> Array a b
hist bnds is = accumArray (+) 0 bnds [(i, 1) | i <- is, inRange bnds i]

decades :: (RealFrac a) => a -> a -> [a] -> Array Int Int
decades a b = hist (0, 9) . map decade
  where decade x = floor ((x-a)*s)
        s = 10/(b-a)

-- class (Ord a) => Ix a where
--     range :: (a, a) -> [a]
--     index :: (a, a) -> a -> Int
--     inRange :: (a, a) -> a -> Bool

-- range (0, 4) => [0, 1, 2, 3, 4]
-- range ((0, 0), (1, 2)) => [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2)]
-- index (1, 9) 2 => 1
-- index ((0, 0), (1, 2)) (1, 1) => 4

-- from matrix of a by b and matrix of b by c to a by c
matMult :: (Ix a, Ix b, Ix c, Num d)
        => Array (a, b) d 
        -> Array (b, c) d 
        -> Maybe (Array (a, c) d)
matMult x y 
  | (lj, uj) == (li', ui') = Just $
    array ((li, lj'), (ui, uj'))
          [((i, j), sum [x!(i, k) * y!(k, j) | k <- range (lj, uj)])
            | i <- range (li, ui),  -- running index for a of matrix 1
              j <- range (lj', uj')] -- running index for c of matrix 2
  | otherwise = Nothing
    where ((li, lj), (ui, uj)) = bounds x
          ((li', lj'), (ui', uj')) = bounds y

matMult' :: (Ix a, Ix b, Ix c, Num d)
         => Array (a, b) d
         -> Array (b, c) d
         -> Maybe (Array (a, c) d)
matMult' x y
  | (lj, uj) == (li', ui') = Just $
      accumArray (+) 0 ((li, lj'), (ui, uj'))
                 [((i, j), x!(i, k) * y!(k, j))
                    | i <- range (li, ui),
                      j <- range (lj', uj'),
                      k <- range (lj, uj)]
  | otherwise = Nothing
  where ((li, lj), (ui, uj)) = bounds x
        ((li', lj'), (ui', uj')) = bounds y


type Cord = (Int, Int)
matrix1 :: Array Cord Int
matrix1 = array ((0, 0), (1, 2)) 
                [ ((0, 0), 1)
                , ((0, 1), 2)
                , ((0, 2), 3)
                , ((1, 0), 2)
                , ((1, 1), 3)
                , ((1, 2), 4)
                ]

matrix2 :: Array Cord Int
matrix2 = array ((0, 0), (2, 0))
                [ ((0, 0), 3)
                , ((1, 0), 4)
                , ((2, 0), 5)
                ]

matrix3 :: Array Cord Int
matrix3 = array ((0, 0), (2, 3))
                [ ((0, 0), 1)
                , ((0, 1), 2)
                , ((0, 2), 3)
                , ((0, 3), 4)
                , ((1, 0), 7)
                , ((1, 1), 8)
                , ((1, 2), 9)
                , ((1, 3), 10)
                , ((2, 0), 13)
                , ((2, 1), 14)
                , ((2, 2), 15)
                , ((2, 3), 16)
                ]

type Matrix a = Array Cord a

mkMatrix :: [[a]] -> Maybe (Matrix a)
mkMatrix input@((c:cols):rows)
  | wellFormatted = Just $ array vertices $ zip (range vertices) (concat input)
    where nr = 1 + length rows
          nc = 1 + length cols
          wellFormatted = all (== nc) (map length rows)
          vertices = ((0, 0), (nr-1, nc-1))
mkMatrix _ = Nothing

matrix1' = mkMatrix [ [1, 2, 3] 
                    , [2, 3, 4] 
                    ]
                      
matrix2' = mkMatrix [ [3]
                    , [4]
                    , [5]
                    ]

matrix3' = mkMatrix [ [1, 2, 3, 4]
                    , [7, 8, 9, 10]
                    , [13, 14, 15, 16]
                    ]

showListStr :: Show a => [a] -> String
showListStr = intercalate " " . map show

showMatrix :: Show a => Matrix a -> String
showMatrix m = intercalate "\n" $ map (showListStr . map (m!)) cordsByRow 
    where byRow = on (==) fst
          cordsByRow = (groupBy byRow . range . bounds) m

putStrMatrix :: (Show a) => Matrix a -> IO ()
putStrMatrix = putStrLn . showMatrix

rotate' :: Int -> [a] -> [a] -- this works for empty list input
rotate' r xs = map snd $ zip xs xs'
    where xs' = drop r (cycle xs)

rotate :: Int -> [a] -> [a] -- make it O(length a)
rotate _ [] = []
rotate r xs = rotate' (r `mod` n) xs
    where n = length xs

type CordR = (Cord, Cord)

legalLayer :: CordR -> Bool
legalLayer ((i1, j1), (i2, j2))
  | i1 > i2 || j1 > j2 = False
  | otherwise = True

-- make layer from (i0, i0) to (ii, jj)
mkLayerCords :: CordR -> [Cord]
mkLayerCords ((i0, j0), (ii, jj))
  | not $ legalLayer ((i0, j0), (ii, jj)) = []
  | i0 == ii && j0 == jj = [(i0, j0)]
  | otherwise =
      (fmap (\i -> (i, j0)) [i0..ii-1]) ++
      (fmap (\j -> (ii, j)) [j0..jj-1]) ++
      (fmap (\i -> (i, jj)) (reverse [i0+1..ii])) ++
      (fmap (\j -> (i0, j)) (reverse [j0+1..jj]))

rotateLayer :: Int -> [Cord] -> Array Cord a -> Array Cord a
rotateLayer r ps m = m // (zip (rotate r ps) (map (m!) ps))

rotateLayers :: Int -> Array Cord a -> Array Cord a
rotateLayers r m = foldl go m layerCords
    where layerCords = (map mkLayerCords . takeWhile legalLayer . iterate next . bounds) m
          next ((i1, j1), (i2, j2)) = ((i1+1, j1+1), (i2-1, j2-1))
          go :: Array Cord a -> [Cord] -> Array Cord a
          go m1 ps = rotateLayer r ps m1
