{-# LANGUAGE GeneralizedNewtypeDeriving #-}
module Supply 
    ( Supply 
    , next
    , runSupply
    , randomsIO ) where


import Control.Monad.State
import System.Random hiding (next)

--a Supply type hides the implementation details of using a State monad
--that returns a type and tracking [s] state
--the deriving part is important to make the code compile with some use cases
newtype Supply s a = S (State [s] a) deriving (Functor, Monad, Applicative)

next :: Supply s (Maybe s) -- a wrapper of State [s] (Maybe s)
next = S $ do 
    st <- get -- get the state, st :: [s]
    case st of 
        [] -> return Nothing
        (x:xs) -> do 
            put xs -- update state
            return (Just x)

runSupply :: Supply s a -> [s] -> (a, [s])
runSupply (S m) xs = runState m xs

-- :t getStdRandom
-- getStdRandom :: (StdGen -> (a, StdGen)) -> IO a

randomsIO :: Random a => IO [a]
randomsIO = getStdRandom $ \g -> 
    let (a, b) = split g
    in (randoms a, b)


showTwo :: (Show s) => Supply s String
showTwo = do
    a <- next
    b <- next
    return ("a: " ++ show a ++ ", b: " ++ show b)


testSupply :: IO ()
testSupply = do 
    print $ runSupply next [1, 2, 3]
    print $ runSupply (liftM2 (,) next next) [1, 2, 3]
    print $ runSupply (liftM2 (,) next next) [1]
    print =<< ((fst . runSupply next) `fmap` randomsIO :: IO (Maybe Integer))
