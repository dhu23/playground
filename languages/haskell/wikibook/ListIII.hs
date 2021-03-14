and' :: [Bool] -> Bool
and' [] = True
and' (False:_) = False
and' (True:bs) = and' bs

and'' :: [Bool] -> Bool
and'' = foldr step True
    where
    step b True = b
    step _ False = False

or' :: [Bool] -> Bool
or' [] = False
or' (True:_) = True
or' (False:bs) = or' bs

or'' :: [Bool] -> Bool
or'' = foldr step False
    where
    step b False = b
    step _ True = True

reverse' :: [a] -> [a]
reverse' = foldl (flip (:)) [] 
