-- Haskell playground

import qualified Data.Map
import qualified Data.Set

helloWorld :: IO()
helloWorld = putStrLn "Hello World"

-- reverse polish notation calculator
-- input is a string, such as "10 4 3 + 2 * -", it should return an int result

-- converts to ["-", "*", "2", "+", "3", "4", "10"]
reversePolishNotation :: String -> [String]
reversePolishNotation = reverse . words

rpn :: (Int -> Int -> Int) -> String -> String -> String
rpn op n1Str n2Str = show (n2 `op` n1)
    where n1 = read n1Str :: Int
          n2 = read n2Str :: Int

opOnStrings :: (Int -> Int -> Int) -> [String] -> [String]
opOnStrings op [] = error ("operation without oprands")
opOnStrings op [only] = error ("operation with only one oprand")
opOnStrings op (n1Str:n2Str:rest) = (rpn op n1Str n2Str):rest

operation :: String -> Maybe (Int -> Int -> Int)
operation "+" = Just (+)
operation "-" = Just (-)
operation "*" = Just (*)
operation _ = Nothing

calcRPN :: [String] -> [String]
calcRPN [] = []
calcRPN [first] = [first]
calcRPN [first, second] = [first, second]
calcRPN inputs@(first:second:leftover) = 
    let maybeOp = operation first
    in case maybeOp of
       Just op -> opOnStrings op (calcRPN (second:leftover))
       Nothing -> first:(calcRPN (second:leftover))


-- alternative method using foldl
foldInput :: (Num a, Read a) => [a] -> String -> [a]
foldInput (x:y:rest) "+" = (x+y):rest
foldInput (x:y:rest) "-" = (y-x):rest
foldInput (x:y:rest) "*" = (x*y):rest
foldInput stack n = (read n):stack

solveRPN :: (Num a, Read a) => String -> a
solveRPN input 
    | null final = error ("WHAT????!!1?")
    | (length final) > 1 = error ("more than one number")
    | otherwise = head final
    where final = foldl foldInput [] (words input)


rpnTest1 = calcRPN ["-", "4", "10"]
rpnTest2 = calcRPN ["+", "3", "5"]
rpnTest3 = calcRPN ["-", "+", "3", "5", "10"]
rpnTest4 = calcRPN ["-", "3", "+", "*", "2", "5", "1"]
rpnTest5 = calcRPN ["-", "*", "2", "+", "3", "4", "10"]

testRPN = do 
    print $ rpnTest1
    print $ rpnTest2
    print $ rpnTest3
    print $ rpnTest4
    print $ rpnTest5
    print $ solveRPN "10 4 3 + 2 * -"
    print $ solveRPN "10.5 4 3 + 2 * -"

-- Heathrow to London
-- find the shortest path from one city to another
-- Heathrow   A --50---A1--5---A2--40--A3---10--A4  London
-- Airport             |       |       |
-- on Left             30      20      25
-- (A, or B)           |       |       |
-- choose one B --10---B1--90--B2--2---B3---8---B4  city

data Node = Node String deriving (Show, Ord, Eq)
data Road = Road Node Node Int deriving (Show, Ord, Eq)
type RoadMap = Data.Map.Map Node [Road]

roadMap :: RoadMap
roadMap = Data.Map.fromList 
    [(heathrow, [heathrowToA1, heathrowToB1])
    ,(a1, [a1ToA2, a1ToB1])
    ,(b1, [b1ToA1, b1ToB2])
    ,(a2, [a2ToA3, a2ToB2])
    ,(b2, [b2ToB3, b2ToA2])
    ,(a3, [a3ToLondon, a3ToB3])
    ,(b3, [b3ToLondon, b3ToA3])]
    where heathrow = Node "Heathrow"
          a1 = Node "A1"
          a2 = Node "A2"
          a3 = Node "A3"
          b1 = Node "B1"
          b2 = Node "B2"
          b3 = Node "B3"
          london = Node "London"
          heathrowToA1 = Road heathrow a1 50
          heathrowToB1 = Road heathrow b1 10
          a1ToB1 = Road a1 b1 30
          b1ToA1 = Road b1 a1 30
          a1ToA2 = Road a1 a2 5
          b1ToB2 = Road b1 b2 90
          a2ToB2 = Road a2 b2 20
          b2ToA2 = Road b2 a2 20
          a2ToA3 = Road a2 a3 40
          b2ToB3 = Road b2 b3 2
          a3ToB3 = Road a3 b3 25
          b3ToA3 = Road b3 a3 25
          a3ToLondon = Road a3 london 10
          b3ToLondon = Road b3 london 8



-- main a map which stores information like for each node, the corresponding
-- previous node in the shortest path to reach this node from source and the 
-- total path distance
-- map Node (Node, Distance)
-- and also keep a queue of edges for BFS of the graph
type ShortestPath = Data.Map.Map Node (Node, Int)
type RoadQueue = [Road]
type VisitedRoad = Data.Set.Set Road
type BFSInfo = (RoadQueue, VisitedRoad)

updateShortestPath :: ShortestPath -> Road -> ShortestPath
updateShortestPath sp (Road fromNode toNode distance)
    | Data.Map.null sp = error ("EMPTY")
    | otherwise = 
        let foundFromNode = Data.Map.lookup fromNode sp
            foundToNode = Data.Map.lookup toNode sp
        in case (foundFromNode, foundToNode) of 
            (Nothing, _) -> error ("didn't find fromNode")
            (Just (_, disToFromNode), Nothing) -> 
                Data.Map.insert toNode (fromNode, distance+disToFromNode) sp
            (Just (_, disToFromNode), Just (ne, d)) ->
                if (distance+disToFromNode) < d
                then Data.Map.insert toNode (fromNode, distance+disToFromNode) sp
                else Data.Map.insert toNode (ne, d) sp


updateBFSRoadQueue :: BFSInfo -> RoadMap -> (BFSInfo, Maybe Road)
updateBFSRoadQueue bfs@([], _) _ = (bfs, Nothing)
updateBFSRoadQueue (r@(Road _ t _):rs, visited) rm =
    let roads = Data.Map.lookup t rm
    in case roads of
        Nothing -> ((rs, visited), Just r)
        Just [] -> ((rs, visited), Just r)
        Just connections -> 
            let connectionSet = Data.Set.fromList connections
                toVisit = Data.Set.difference connectionSet visited
                newQ = rs++(Data.Set.toList toVisit)
                newVisited = Data.Set.insert r visited
            in ((newQ, newVisited), Just r)


solver :: RoadMap -> (ShortestPath, BFSInfo) -> (ShortestPath, BFSInfo)
solver _ (sp, bfs@([], _)) = (sp, bfs)
solver rm (sp, bfs@(q, visited)) = 
    let ((newQ, newVisited), processedRoad) = updateBFSRoadQueue bfs rm 
    in case processedRoad of
        Nothing -> error ("BAD. sp="++(show sp)++" ,bfs="++show(bfs))
        Just r -> let newSp = updateShortestPath sp r
                  in solver rm (newSp, (newQ, newVisited))


solveShortestPath :: RoadMap -> Node -> Node -> ShortestPath
solveShortestPath rm s t = 
    let startPath = Data.Map.fromList [(s, (s, 0))]
        roads = Data.Map.lookup s rm
        visited = Data.Set.empty
        (shortestPath, _) = case roads of 
            Nothing -> error ("No roads")
            Just rs -> solver rm (startPath, (rs, visited)) 
    in shortestPath


testRoadMap = do 
    print roadMap

testShortestPath = do
    let heathrow = Node "Heathrow"
        a1 = Node "A1"
        a2 = Node "A2"
        a3 = Node "A3"
        b1 = Node "B1"
        b2 = Node "B2"
        b3 = Node "B3"
        london = Node "London"
        heathrowToA1 = Road heathrow a1 50
        heathrowToB1 = Road heathrow b1 10
        a1ToB1 = Road a1 b1 30
        b1ToA1 = Road b1 a1 30
        a1ToA2 = Road a1 a2 5
        b1ToB2 = Road b1 b2 90
        a2ToB2 = Road a2 b2 20
        b2ToA2 = Road b2 a2 20
        a2ToA3 = Road a2 a3 40
        b2ToB3 = Road b2 b3 2
        a3ToB3 = Road a3 b3 25
        b3ToA3 = Road b3 a3 25
        a3ToLondon = Road a3 london 10
        b3ToLondon = Road b3 london 8    
        startPath = Data.Map.fromList [(heathrow, (heathrow, 0))] 
        updatePath = foldl updateShortestPath startPath
    print $ updatePath [heathrowToA1]
    print $ updatePath [heathrowToA1, a1ToB1]
    print $ updatePath [heathrowToA1, a1ToB1, a1ToA2]
    print $ updatePath [heathrowToA1, a1ToB1, a1ToA2, heathrowToB1]
    print $ updatePath [heathrowToA1, a1ToB1, a1ToA2, heathrowToB1, b1ToA1]
    print "-------------------------"
    print $ solveShortestPath roadMap heathrow london



main = 
    --testShortestPath
    testRPN 
