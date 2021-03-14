-- stack has the following function
-- pop :: m a
-- top :: m a
-- push :: a -> m ()

-- use list for stack underlying data
import Control.Monad.State

type Stack a = [a]

emptyStack = []

pop :: State (Stack a) (Maybe a)
pop = do 
    s <- get
    case s of
        [] -> put [] >> return Nothing
        (x:xs) -> put xs >> return (Just x)

push :: a -> State (Stack a) ()
push x = do
    s <- get
    put (x:s)

top :: State (Stack a) (Maybe a)
top = do
    s <- get
    case s of 
        [] -> return Nothing
        (x:xs) -> return (Just x)

-- queue has the following method
-- dequeue :: m a
-- enqueue :: a -> m a
-- front :: m a
-- back :: m a

data Queue a = Queue { front :: Stack a, back :: Stack a } deriving Show

emptyQueue = Queue [] []

reconstruct :: Queue a -> Queue a
reconstruct (Queue [] bs) = Queue (reverse bs) []
reconstruct q = q

dequeue :: State (Queue a) (Maybe a)
dequeue = do
    q <- fmap reconstruct get
    case front q of
        [] -> put q >> return Nothing
        (x:xs) -> put q { front = xs } >> return (Just x)

enqueue :: a -> State (Queue a) ()
enqueue a = do
    q <- get
    let bs = back q
    put q { back = (a:bs) }
    

