import qualified Data.Map as DM
import qualified Data.Set as DS
import qualified Data.Array as DA
import qualified Data.List as DL
import qualified Data.Function as DF
import qualified Control.Monad.State as CMS

type Point = (Int, Int) -- row, column, effective (y, x)
-- (row, column) makes using 2D-Array to depict the plane more naturally
type Area = DS.Set Point

mergeArea :: Area -> Area -> Area
mergeArea = DS.union

mergeAreas :: [Area] -> Area
mergeAreas = foldr DS.union DS.empty

data LineAlign = LHorizontal | LVertical deriving (Show, Eq)
data Line = Line { lineAlign :: LineAlign
                 , linePos :: Int
                 , lineSpan :: (Int, Int)
                 } deriving Show

-- IRT: Isosceles right triangle
data IRTAlign = IRTUp | IRTDown | IRTLeft | IRTRight deriving (Show, Eq)
data IRT = IRT { irtAlign :: IRTAlign
               , irtPos :: Point 
               , irtHeight :: Int
               } deriving Show

baseOfIRT :: IRT -> Line
baseOfIRT (IRT IRTUp (r, c) h) = Line LHorizontal (r+h) (c-h, c+h)
baseOfIRT (IRT IRTDown (r, c) h) = Line LHorizontal (r-h) (c-h, c+h)
baseOfIRT (IRT IRTLeft (r, c) h) = Line LVertical (c+h) (r-h, r+h)
baseOfIRT (IRT IRTRight (r, c) h) = Line LVertical (c-h) (r-h, r+h)

type Canvas = DA.Array Point Char

drawP :: Char -> Point -> Canvas -> Canvas
drawP x p canvas = if (DA.bounds canvas) `DA.inRange` p
                    then canvas DA.// [(p, x)]
                    else canvas

drawA :: Char -> Area -> Canvas -> Canvas
drawA x a canvas = foldr (drawP x) canvas a

draw :: Drawable a => Char -> a -> Canvas -> Canvas
draw x a canvas = drawA x (fill a) canvas

drawC :: Canvas -> Canvas -> Canvas
drawC c1 c2 = c1 DA.// (DA.assocs c2)

drawM_ :: (Canvas -> Canvas) -> CMS.State Canvas ()
drawM_ f = CMS.state $ \s -> ((), f s)

drawPM_ x = drawM_ . drawP x

drawAM_ x = drawM_ . drawA x

drawCM_ = drawM_ . drawC

class Drawable a where
  fill :: a -> Area
  move :: (Int, Int) -> a -> a

instance Drawable Line where
  fill (Line LHorizontal r (c1, c2)) 
    | c1 <= c2 = DS.fromList [(r, c) | c <- [c1..c2]]
    | otherwise = fill (Line LHorizontal r (c2, c1))
  fill (Line LVertical c (r1, r2))
    | r1 <= r2 = DS.fromList [(r, c) | r <- [r1..r2]]
    | otherwise = fill (Line LVertical c (r2, r1))

  move (dr, dc) (Line LHorizontal r (c1, c2)) = 
      Line LHorizontal (r+dr) ((c1+dc), (c2+dc))
  move (dr, dc) (Line LVertical c (r1, r2)) = 
      Line LVertical (c+dc) ((r1+dr), (r2+dr))

instance Drawable IRT where
  fill (IRT align p h) = 
      mergeAreas $ map (fill . baseOfIRT . IRT align p) [0..h]

  move (dr, dc) (IRT align p h) = IRT align (moveP (dr, dc) p) h

emptyCanvas x (p1, p2) = DA.array (p1, p2) ps
  where
    ps = zip (DA.range (p1, p2)) (repeat x)

putCanvas :: Canvas -> IO ()
putCanvas canvas = putStrLn pic
  where 
    rows = DL.groupBy ((==) `DF.on` (fst . fst)) . DA.assocs
    pic = DL.intercalate "\n" $ map (map snd) (rows canvas)

moveP :: (Int, Int) -> Point -> Point
moveP (dr, dc) (r, c) = (r+dr, c+dc)

moveA (dr, dc) = map (move (dr, dc))

type SierLevel = Int 
-- 0 means base triangle of height h, 2^0 (h+1)
-- 1 means 3 base trangles of height h, 2^1 (h+1)

mkSier :: SierLevel -> IRT -> [IRT]
mkSier level irt = snd $ (iterate sier (h0, [irt])) !! level
  where
    h0 = 1+irtHeight irt
    align = irtAlign irt

    dupDirs h IRTUp = [(h, -h), (h, h), (0, 0)]
    dupDirs h IRTDown = [(-h, h), (-h, -h), (0, 0)]
    dupDirs h IRTLeft = [(h, h), (-h, h), (0, 0)]
    dupDirs h IRTRight = [(-h, -h), (h, -h), (0, 0)]

    dup irts dir = fmap (move dir) irts
    sier (h, irts) = ( 2*h
                     , concat (map (dup irts) (dupDirs h align))
                     )

drawSier canvas x level irt = putCanvas $ drawA x area canvas
  where 
    area = mergeAreas $ map fill $ mkSier level irt

drawSier1 level h = drawSier canvas '1' level (IRT IRTUp (0, 0) h)
  where
    hc = 2^level*(h+1)
    wc = hc-1
    canvas = emptyCanvas '_' ((0, -wc), (hc-1, wc))

drawSier2 level h = drawSier canvas '1' level (IRT IRTLeft (wc, 0) h)
  where
    hc = 2^level*(h+1)
    wc = hc-1
    canvas = emptyCanvas '_' ((0, 0), (2*wc, hc-1))

drawSier1_ :: Int -> IO ()
drawSier1_ rowN = do
    let levels = [0..]
        hs = map (subtract 1) . takeWhile (<= rowN) . iterate (2*) $ 1
    sequence_ $ fmap (uncurry drawSier1) $ zip levels (reverse hs)
