-- implement a function rollNDiceIO :: Int -> IO [Int] that given an integer
-- returns a list of that number of pseudo-random integers between 1 and 6

import System.Random (randomRIO, randomR, RandomGen, StdGen, random)

rollDiceIO :: IO Int
rollDiceIO = randomRIO (1, 6)

rollNDiceIO n = sequence $ replicate n rollDiceIO


rollDice :: (RandomGen g) => g -> (Int, g)
rollDice = randomR (1, 6)

rollTwoDice :: StdGen -> ((Int, Int), StdGen)
rollTwoDice gen = ((n, m), gen2)
    where
    (n, gen1) = rollDice gen
    (m, gen2) = rollDice gen1
    
-- reimplement these instances by using explicitly the Sate constructors
-- and functions instead of relying on the Monad instance


newtype State s a = State { runState :: s -> (a, s) }

-- fmap :: (a -> b) -> m a -> m b
-- or (a -> b) -> State s a -> State s b
instance Functor (State s) where
    fmap f p = State $ \ s -> let (a, s') = runState p s in (f a, s')

-- Control.Applicative.pure :: Applicative f => a -> f a
-- or a -> (State s a)a
--
-- <*> :: Applicative f => f (a -> b) -> f a -> f b
-- or State s (a -> b) -> State s a -> State s b
-- after unwrapping, (s -> (a -> b, s)) -> (s -> (a, s)) -> (s -> (b, s))
instance Applicative (State s) where
    pure a = State $ \ s -> (a, s)
    f <*> p = State $ \ s ->
        let (f', s1) = runState f s
            (a, s2) = runState p s1
        in (f' a, s2)

-- >>= :: Monad m => m a -> (a -> m b) -> m b
-- or State s a -> (a -> State s b) -> State s b
instance Monad (State s) where
    return = pure
    p >>= k = State $ \ s ->
        let (a, s1) = runState p s
        in runState (k a) s1
 

rollTwoDice' :: StdGen -> ((Int, Int), StdGen)
rollTwoDice' = runState $ (,) <$> (State rollDice) <*> (State rollDice)

-- similarly to what was done for rollNDiceIO, implement rollNDice :: 
-- Int -> State StdGen [Int] that given an integer, returns a list with
-- that number of pseudo-random integers between 1 and 6
rollNDice :: Int -> State StdGen [Int]
rollNDice n = sequence $ replicate n (State rollDice)

put s = State $ \ _ -> ((), s)
get = State $ \ s -> (s, s)

-- besides put and get, there are also
-- modify :: (s -> s) -> State s () which modifies the current state with
-- a function, and
-- gets :: (s -> a) -> State s a
-- which produces a modified copy of the state while leaving the state 
-- itself unchanged

modify :: (s -> s) -> State s ()
modify f = do
    s <- get
    put (f s)

gets f = do
    s <- get
    return (f s)

-- if you are not convinced that State is worth using, try to implement
-- a functin equivalent to evalState allTypes without making use of
-- monads, i.e. with an approach similar to clumsyRollDice above
clumsyGetAllTypes :: StdGen 
                  -> (Int, Float, Char, Integer, Double, Bool, Int)
clumsyGetAllTypes g = (x1, x2, x3, x4, x5, x6, x7)
    where 
    (x1, g1) = random g
    (x2, g2) = random g1
    (x3, g3) = random g2
    (x4, g4) = random g3
    (x5, g5) = random g4
    (x6, g6) = random g5
    (x7, g7) = random g6
