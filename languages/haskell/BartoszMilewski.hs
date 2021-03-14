--      p 
--  a -----> b    partial function        a -> Maybe b
--
--      e
--  a -----> b    global variable         a -> (e -> b)
--
--      s
--  a -----> b    state change            a -> (s -> (b, s))
--
--      i
--  a -----> b    indeterministic         a -> [b]
--
--     imp
--  a -----> b                            a -> m b
--
--
--  identity is resembled by Monad return

{--
data Identity a = Id a

runIdentity (Id x) = x

instance Monad Identity where
    return x = Id x
    ix >>= f = f (runIdentity ix)

data Eval a = Done a

runEval (Done a) = a

instance Monad Eval where
    return x = Done x
    -- like identity monad except that the unpacking is forced, ie strict
    (Done x) >>= f = f x
--}

import Control.Exception
import Control.Parallel.Strategies

test = do 
    a <- rpar (fib 40)
    b <- rpar (fib 41)
    return (a, b)

fib :: Int -> Integer
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

main = do
    r <- evaluate $ runEval test
    print r
