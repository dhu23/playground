data Weird a b = First a 
               | Second b 
               | Third [(a, b)] 
               | Fourth (Weird a b)
    deriving (Show)


weirdMap :: (a -> c) -> (b -> d) -> Weird a b -> Weird c d
weirdMap fa fb = g
    where
        g (First a) = First (fa a)
        g (Second b) = Second (fb b)
        g (Third ws) = Third (map (\ (a, b) -> (fa a, fb b)) ws)
        g (Fourth w) = Fourth (g w)

-- First :: a -> Weird a b
-- Second :: b -> Weird a b
-- Third :: 
