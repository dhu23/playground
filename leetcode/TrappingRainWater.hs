
type Position = Int
type Height = Int
type Bar = (Position, Height)

type Collected = Int
type Tentative = Int
type Trapper = ([Bar], Collected, Tentative)


addBars :: Trapper -> Bar -> Trapper
addBars ([], c, _) b = ([b], c, 0)
addBars ([b1@(p1, h1)], c, t) b0@(p0, h0) = 
    if h0 >= h1     
    then let c' = c+t-(h0-h1)*(p0-p1-1)
         in ([b0], c', 0)            
    else ([b0, b1], c+t, 0)
addBars (b1@(p1, h1):b2@(p2, h2):bars, c, t) b0@(p0, h0) = 
    if h0 < h1
    then (b0:b1:b2:bars, c, 0)
    else let t' = t+(h0-h1)*(p1-p2) 
         in addBars ((b2:bars), c, t') b0

solveTrapper :: [Int] -> Trapper
solveTrapper xs = foldl addBars ([], 0, 0) $ zip [0..] xs

printSolver :: [Int] -> IO()
printSolver bs = putStrLn $ show $ solveTrapper bs

main = do
    printSolver [1] -- 0
    printSolver [1, 0] -- 0
    printSolver [1, 0, 1] -- 1
    printSolver [3, 2, 1] -- 0
    printSolver [3, 2, 1, 2] -- 1
    printSolver [3, 2, 1, 2, 3] -- 4
    printSolver [0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1] -- 6
    printSolver [1, 2, 3, 2, 1] -- 0
