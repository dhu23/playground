import System.Random
import Control.Monad.State


data MyType = MT Int Bool Char Int deriving Show

makeRandomValue :: StdGen -> (MyType, StdGen)
makeRandomValue g = let (n, g1) = randomR (1, 100) g
                        (b, g2) = random g1
                        (c, g3) = randomR ('a', 'z') g2
                        (m, g4) = randomR (-n, n) g3
                    in (MT n b c m, g4)


getAny :: (Random a) => State StdGen a
getAny = do
    g <- get
    (x, g') <- return $ random g
    put g'
    return x

getOne :: (Random a) => (a, a) -> State StdGen a
getOne bounds = do
    g <- get
    (x, g') <- return $ randomR bounds g
    put g'
    return x

makeRandomValueST :: StdGen -> (MyType, StdGen)
makeRandomValueST = runState $ do 
    n <- getOne (1, 100)
    b <- getAny
    c <- getOne ('a', 'z')
    m <- getOne (-n, n)
    return (MT n b c m)

main = do
    print $ makeRandomValue $ mkStdGen 0
    print $ makeRandomValueST $ mkStdGen 0
