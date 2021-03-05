import qualified Data.Array as DA (Array, listArray, (//), assocs)
import qualified Data.List as DL (groupBy, intercalate)
import qualified Data.Function as DF (on)

type Point = (Int, Int)
data Triangle = Triangle { up :: Point, height :: Int } deriving Show
type Canvas = DA.Array Point Char

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

split hLvl (r, c) = let w = 2^(hLvl-1) in [(r, c), (r+w, c-w), (r+w, c+w)]

splitSier maxLevel (sierLevel, ts) = (sierLevel+1, ts')
  where
    ts' = ts >>= (split (maxLevel-sierLevel))

mkSierpinski :: Int -> Int -> Canvas
mkSierpinski maxLevel sierLevel
  | maxLevel >= sierLevel = foldr drawTriangle c $ allTriangles
  | otherwise = c
  where
    c = emptyCanvas maxLevel
    (l', ts') = head $ drop sierLevel $ iterate (splitSier maxLevel) (0, [(0, 0)])
    allTriangles = map (\p -> Triangle p (2^(maxLevel-l'))) ts'

main = do 
    sierLevel <- readLn
    drawCanvas $ mkSierpinski 5 sierLevel
