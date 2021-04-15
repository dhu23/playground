
type Position = (Int, Int)
type Board = [Position]

availableSlots :: Int -> Board -> [Position]
availableSlots n b = [(x, y) | x <- [0..n-1], y <- [0..n-1], legal x y]
    where legalX = not . (`elem` (map fst b))
          legalY = not . (`elem` (map snd b))
          legalLeftDiag = not . (`elem` (map (\(x, y) -> x-y) b))
          legalRightDiag = not . (`elem` (map (\(x, y) -> x+y) b))
          legal x y = legalX x && legalY y && 
                      legalLeftDiag (x-y) && legalRightDiag (x+y)


nQueens :: Int -> [Board]
nQueens n = solveNQueens n []
    where solveNQueens n b
            | length b == n = [b]
            | otherwise = concat $ map (solveNQueens n) possibleBoards
                where possibleBoards = map (:b) (availableSlots n b)

main :: IO ()
main = do 
    print $ nQueens 4
    print $ take 1 $ nQueens 8
