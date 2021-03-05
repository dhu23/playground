import Data.List (groupBy, sortBy, intercalate)

-- a triangle is defined by its vertices. (Int, Int) 
type Point = (Int, Int)
data Triangle = Triangle
              { upper :: Point
              , left :: Point
              , right :: Point
              , height :: Int } deriving (Show)

 
-- make a triangle from its upper vertex and its height
makeTriangle :: Point -> Int -> Triangle
makeTriangle upperVertex@(ux, uy) h
    | h > 1 && h `mod` 2 /= 0 = error ("no triangle with height " ++ show h)
    | otherwise = Triangle { upper=upperVertex
                           , left=leftVertex
                           , right=rightVertex
                           , height=h }
    where leftVertex = (ux-h+1, uy-h+1)
          rightVertex = (ux+h-1, uy-h+1)

getSection :: Int -> Triangle -> (Int, Int)
getSection h t
    | h < 1 || h > height t = error ("section out side of triangle:" ++ show h)
    | otherwise = let (ux, uy) = upper t in (ux-h+1, ux+h-1)

-- returned triangles are sorted by their position from upper to bottom, 
-- and left to right
split :: Triangle -> [Triangle]
split t
    | h < 2 = error ("cannot split triangle with height less then 2")
    | h `mod` 2 /= 0 = error ("triangle height not multiplier of 2")
    | otherwise = [ upperOne
                  , (makeTriangle lUpperVertex h')
                  , (makeTriangle rUpperVertex h') ]
    where h = height t
          h' = h `div` 2
          upperOne = makeTriangle (upper t) h'
          lUpperVertex = let (x, y) = left upperOne in (x-1, y-1)
          rUpperVertex = let (x, y) = right upperOne in (x+1, y-1)

toWidth h = 2*h-1

triangleOrder :: Triangle -> Triangle -> Ordering
triangleOrder t1 t2
    | height t1 < height t2 = LT
    | height t2 > height t2 = GT
    | otherwise = if uy1 /= uy2 then flip compare uy1 uy2 else ux1 `compare` ux2
    where (ux1, uy1) = upper t1
          (ux2, uy2) = upper t2

-- total height -> iteration -> triangles 
sierpinski :: Int -> Int -> [Triangle]
sierpinski h 0 = [makeTriangle (h, h) h] 
sierpinski h n = concat $ map split $ sierpinski h (n-1)


groupTriangles ts = groupBy f $ sortBy triangleOrder ts
    where f t1 t2 = let (_, y1) = upper t1
                        (_, y2) = upper t2
                    in y1 == y2

type Picture = [[Char]]

makeCanvas :: Int -> Picture
makeCanvas h = replicate h $ replicate w '_'
    where w = toWidth h

drawPicture :: Picture -> IO ()
drawPicture picture = putStrLn $ intercalate "\n" picture

makeAscii :: Int -> [Triangle] -> Picture
makeAscii h ts = concat $ map drawGroup ts' 
    where ts' = groupTriangles ts
          w = toWidth h
          --tGroup is a group of triangles at the same height
          drawGroup tGroup = map draw [1..groupH]
                where groupH = height $ head tGroup
                      drawLine 0 _ = []
                      drawLine col [] = '_' : (drawLine (col-1) []) 
                      drawLine col intervals@((start, end):(ints))
                        | pos < start = '_' : (drawLine (col-1) intervals)
                        | pos > end = drawLine col ints
                        | otherwise = '1' : (drawLine (col-1) intervals)    
                        where pos = w - col + 1
                      draw l = drawLine w $ map (getSection l) tGroup

main = do
    n <- readLn :: IO Int
    drawPicture $ makeAscii 32 $ sierpinski 32 n

