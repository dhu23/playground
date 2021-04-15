import Data.Array 
import qualified Data.List as DL

type TicTacToe = Array Int Char

makeGame :: Int -> TicTacToe
makeGame n = listArray (0, n*n-1) $ repeat ' '

allTheSame :: [a] -> Maybe a
allTheSame [] = Nothing
allthesame (x:xs) = if all (==x) xs then Just x else Nothing

hasWon :: TicTacToe -> Maybe Char
hasWon game = undefined
    where (start, end) = bounds game
          n = end-start+1
          row r = [n*r..n*r+n-1]
          column c = [c, n+c..(n-1)*n+c]
          leftDiag = [i*n+i | i <- [0..n-1]]
          rightDiag = [i*n+(n-1-i) | i <- [0..n-1]]
          get = map (game !)

main = do 
    print $ makeGame 3
