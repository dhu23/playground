module Queue where

data Queue a = Queue [a] [a] deriving Show

-- keep queue in two parts so that front and back access is fast
-- if size is 1, keep the element in first list
-- if the first list is empty while the second is not, move (n-1) items there

emptyQueue = Queue [] []

isEmpty (Queue [] []) = True
isEmpty _ = False

front :: Queue a -> Maybe a
front (Queue (f:fs) _) = Just f
front (Queue [] bs'@(b:bs) = Just (last bs')
front _ = Nothing

back :: Queue a -> Maybe a
back (Queue _ (b:_)) = Just b
back (Queue fs'@(f:fs) []) = Just (last fs')
back _ = Nothing

enqueue :: a -> Queue a -> Queue a
enqueue x (Queue [] []) = Queue [x] []
enqueue x (Queue [] bs'@(b:bs)) = Queue (reverse bs'@(b:bs)) [x]
enqueue x (Queue fs'@(f:fs) bs) = Queue fs' (x:bs)

dequeue :: Queue a -> (Maybe a, Queue a)
dequeue (Queue [] []) = (Nothing, Queue [] [])
dequeue (Queue [] (b:bs)) = 
    case bs of 
      [] -> (Just b, Queue [] [])
      [x] -> (Just x, Queue [b] [])
      _ -> (Just (last bs), Queue (reverse $ init bs) [b])
dequeue (Queue (f:fs) []) = (Just f, Queue fs [])
dequeue (Queue (f:fs) [b]) = (Just f, Queue 

