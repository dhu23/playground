import qualified Data.Map as DM
import qualified Data.Set as DS
import qualified Data.Array as DA
import qualified Data.List as DL

type Point = (Int, Int)
type HLine = DS.Set Int -- horizontal line
type Graph = DM.Map Int HLine

line :: Int -> Int -> HLine
line x w = DS.fromList [(x-w)..(x+w)]

type HLineA = DA.Array Int Char
emptyLine :: Int -> HLineA
emptyLine w = DA.array ((-w), w) (zip [(-w)..w] $ repeat ' ')

drawHLineA :: HLineA -> String
drawHLineA a = map snd $ DA.assocs a

drawHLine :: Int -> HLine -> String
drawHLine w l = drawHLineA l' 
    where l' = (emptyLine w) DA.// (zip (DS.toAscList l) (repeat '1'))

lineGraph :: Point -> Int -> Graph
lineGraph (x, y) w = DM.insert y (line x w) DM.empty

mergeGraphs :: [Graph] -> Graph
mergeGraphs = DM.unionsWith DS.union

type Drawer = Point -> Int -> Graph
-- runs 
-- lineGraph (x, y) 0
-- lineGraph (x, y+1) 1
-- lineGraph (x, y+2) 2
-- ......
-- then merge them together
triangle :: Drawer
triangle (x, y) h = mergeGraphs graphs
    where blockLine x y h = lineGraph (x, y+h) h
          graphs = (blockLine x y) <$> [0..(h-1)]

sierpinski :: Int -> Drawer
sierpinski tsize (x, y) 1 = triangle (x, y) tsize
sierpinski tsize (x, y) h = mergeGraphs [ sierpinski tsize (x, y) h'
                                        , sierpinski tsize (x-w, y+w) h'
                                        , sierpinski tsize (x+w, y+w) h'
                                        ]
    where h' = h-1
          w = 2^(h'-1)*tsize

-- h=1          1    h' not exit, w=0=1-1
-- h=2          1    h'=1, (x-1, y+1), (x+1, y+1), w=1=2-1
--             1 1
-- h=3          1    h'=2, (x-2, y+2), (x+2, y+2), w=3=4-1
--             1 1
--            1   1
--           1 1 1 1
-- h=4          1    h=3, (x-4, y+4), (x+4, y+4), w=7=8-1
--             1 1
--            1   1
--           1 1 1 1
--          1       1
--         1 1     1 1
--        1   1   1   1
--       1 1 1 1 1 1 1 1

drawSierpinski :: Int -> Int -> String
drawSierpinski tsize h = DL.intercalate "\n"
                       $ ((drawHLine w . snd) <$> (DM.assocs g))
    where g = sierpinski tsize (0, 0) h
          w = 2^(h-1)*tsize-1

