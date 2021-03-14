module StateMonadTransEx where

import System.Random
import Control.Monad.State

-- lift :: (Monad m, MonadTrans t) => m a -> t m a
-- lift a monadic value m a to a new context of t
-- I think this is one of the reasons that monad transformer acquires
-- the form of ExceptIO String IO rather than ExceptIO IO String

-- StateT [Integer] IO a = StateT $ [Integer] -> IO (a, [Integer])
-- this function is the definition of liftIO :: IO a -> m a
io :: IO a -> StateT [Integer] IO a
-- IO a -> ([Integer] -> IO (a, [Integer])
io x = StateT $ f
    where 
    f ns = do
        a <- x
        return (a, ns)

pop :: StateT [Integer] IO Integer
pop = do -- within combined monad context StateT [Integer] IO
    (x:xs) <- get
    put xs
    return x

code :: StateT [Integer] IO ()
code = do -- within combined monad context StateT [Integer] IO
    x <- pop
    lift $ print x -- liftIO $ print x, or io $ print x work too
    y <- pop
    lift $ print y -- liftIO $ print y, or io $ print y works too
    return ()

---------------------------------------------------------------------

guessSession :: Int -> StateT Int IO ()
guessSession answer = do -- StateT Int IO monad
    gs <- lift getLine -- lift/promote IO String to StateT Int IO String
    let g = read gs
    modify (+1) -- StateT Int IO (), modify state without returning value

    case compare g answer of 
        LT -> do
            lift $ putStrLn "Too low" -- turn IO () to StateT Int IO ()
            guessSession answer
        GT -> do
            lift $ putStrLn "Too high"
            guessSession answer
        EQ -> lift $ putStrLn "Got it!"

-----------------------------------------------------------------------

data Vars = Vars { var1 :: Int, var2 :: Float }

type Selector a = (StateT Vars IO a, a -> StateT Vars IO ())

-- gets :: (Monad m) => (s -> a) -> StateT s m a
-- this is basically a getter and a setter/putter function for field var1
s1 :: Selector Int
s1 = ( gets var1 -- turn return type from Vars to var1 field
     , \x -> modify (\vs -> vs {var1 = x}) -- modify takes a Vars -> Vars
     )

s2 :: Selector Float
s2 = ( gets var2
     , \x -> modify (\vs -> vs {var2 = x})
     )

sel :: Selector a -> StateT Vars IO a
sel = fst

-- get field var1 and apply function (a -> a) then put it back
mods :: Selector a -> (a -> a) -> StateT Vars IO ()
mods (gf, uf) mfun = do
    a <- gf
    uf . mfun $ a

sample :: StateT Vars IO ()
sample = do
    a <- sel s1 -- StateT Vars IO (), a is of type Int
    mods s2 (\x -> x * (fromIntegral a)) -- modify var2 field by (*a)
    b <- sel s2 -- extract var2 field, of type Float
    vv <- get
    liftIO $ print b -- print 2nd field from (v1, v1*v2)
    liftIO $ print (var1 vv, var2 vv)

