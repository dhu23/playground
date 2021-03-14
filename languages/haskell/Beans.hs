import System.Random (randomRIO, randomR, StdGen, mkStdGen)
import Control.Monad (liftM, forM, foldM)
import Control.Monad.State (State, get, put, runState, evalState)

multiply :: Int -> a -> [a]
multiply = replicate

split :: a -> State StdGen [a]
split x = do 
    g <- get
    let (n, g') = randomR (0, 2) g
    put g'
    return $ multiply n x

splitIO :: a -> IO [a]
splitIO x = multiply <$> randomRIO (0, 2) <*> pure x

splits :: [a] -> State StdGen [a]
splits = liftM concat . sequence . map split
-- or splits ns = (mapM split ns) >>= (return . concat)

splitsIO :: [a] -> IO [a]
splitsIO = liftM concat . sequence . map splitIO

evalBeans :: StdGen -> State StdGen [a] -> [a]
evalBeans = flip evalState

run :: State StdGen [a] -> [a]
run = evalBeans (mkStdGen 0)

doExperiments :: Int -> [a] -> State StdGen [a]
doExperiments n xs = foldl (>>=) (return xs) (replicate n splits)

doExperimentsIO :: Int -> [a] -> IO [a]
doExperimentsIO n xs = foldl (>>=) (return xs) (replicate n splitsIO)
