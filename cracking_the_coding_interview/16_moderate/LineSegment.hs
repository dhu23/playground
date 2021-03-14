type Point = (Double, Double)

data LineSegment = (Point, Point)

data Relation = Parallel 
              | Identical
              | Overlay LineSegment
              | Intersect Point 
              | Else 
              deriving (Show)

data Line = NonVerticalLine Double Double 
          | VerticalLine Double
          deriving (Show)

lineRelation :: Line -> Line -> Relation
lineRelation (VerticalLine x1) (VerticalLine x2)
    | x1 == x2 = Identical
    | otherwise = Parallel
lineRelation (VerticalLine x0) (NonVerticalLine slope intercept) 
    = Intersect (x0, x0*slope+intercept)
lineRelation nvl@(NonVerticalLine _ _) vl@(VerticalLine _)
    = lineRelation vl nvl
lineRelation (NonVerticalLine slope1 intercept1) 
             (NonVerticalLine slope2 intercept2) 
    | slope1 == slope2 && intercept1 == intercept2 = Identical
    | slope1 == slope2 = Parallel
    | otherwise = Intersect (x, y)
    where x = (intercept2-intercept1)/(slope1-slope2)
          y = slope1*x+intercept1


toLine :: LineSegment -> Line
toLine ((x1, y1), (x2, y2))
    | x1 == x2 && y1 == y2 = error ("Invalid Segment")
    | x1 == x2 = VerticalLine x1
    | otherwise = NonVerticalLine slope intercept
    where slope = (y2-y1)/(x2-x1)
          intercept = y1-x1*slope


lineSegRelation :: LineSegment -> LineSegment -> Relation
lineSegRelation ls1 ls2
    where l1 = toLine ls1
          l2 = toLine ls2
          lr = lineRelation l1 l2

main = do 
    let line1 = NonVerticalLine 1.0 (-1.0)
        line2 = NonVerticalLine (-1.0) 3.0
    print $ lineRelation line1 line2
