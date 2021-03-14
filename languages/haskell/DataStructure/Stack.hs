module Stack 
    ( Stack
    , emptyStack
    , isEmpty
    , top
    , push
    , pop
    , isEmptyM
    , topM
    , pushM
    , popM
    ) where

import Control.Monad.State

data Stack a = Stack [a] deriving Show

emptyStack = Stack []

isEmpty (Stack []) = True
isEmpty _ = False

top (Stack []) = Nothing
top (Stack (x:xs)) = Just x

push x (Stack xs) = Stack (x:xs)

pop (Stack []) = (Nothing, Stack [])
pop (Stack (x:xs)) = (Just x, Stack xs)

isEmptyM :: State (Stack a) Bool
isEmptyM = liftM isEmpty get

topM :: State (Stack a) (Maybe a)
topM = liftM top get

pushM :: a ->  State (Stack a) ()
pushM x = state $ \s -> ((), push x s)

popM :: State (Stack a) (Maybe a)
popM = state pop
