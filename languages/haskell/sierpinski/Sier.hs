import qualified Data.Array as DA (Array, listArray, (//), assocs)
import qualified Data.List as DL (groupBy, intercalate)
import qualified Data.Function as DF (on)

type Point = (Int, Int)
data Sierpinski = Sierpinski { level :: Int -- 5 means 32 rows
                             , tUp :: Point
                             , tHeight :: Int 
                             , tPos :: [Point] } deriving Show 
data Triangle = Triangle { up :: Point, height :: Int } deriving Show
type Canvas = DA.Array Point Char

move (dr, dc) (r, c) = (r+dr, c+dc)

allTriangles (Sierpinski _ _ h ts) = map (\p -> Triangle p h) ts

emptyCanvas maxLevel = DA.listArray ((0, 1-h), (h-1, h-1)) $ repeat '_'
  where 
    h = 2^maxLevel -- 1*(2^maxLevel)

drawTriangle :: Triangle -> Canvas -> Canvas
drawTriangle (Triangle (r, c) h) canvas = foldr drawLine canvas $ map line [1..h]
  where 
    line h = let h' = h-1 in [(r+h', j) | j <- [c-h'..c+h']]
    drawLine l c = c DA.// map (\p -> (p, '1')) l

drawCanvas :: Canvas -> IO ()
drawCanvas canvas = putStrLn pic
  where
    rows = DL.groupBy ((==) `DF.on` (fst . fst)) . DA.assocs
    pic = DL.intercalate "\n" $ map (map snd) (rows canvas)

sier (Sierpinski l p h ts) = Sierpinski (l+1) p h ts'
  where
    w = (2^l)*h
    ts' = ts ++ (fmap (move (w, -w)) ts) ++ (fmap (move (w, w)) ts)

mkSierpinski :: Int -> Int -> Canvas
mkSierpinski maxLevel sierLevel
  | maxLevel >= sierLevel = foldr drawTriangle c $ allTriangles s
  | otherwise = c
  where
    c = emptyCanvas maxLevel
    s0 = let th = 2^(maxLevel-sierLevel) in Sierpinski 0 (0, 0) th [(0, 0)]
    s = head $ drop sierLevel $ iterate sier s0

main = do 
    sierLevel <- readLn
    drawCanvas $ mkSierpinski 5 sierLevel
