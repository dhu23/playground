{-# LANGUAGE ViewPatterns #-}

data Coordinates = Cartesian Int Int 
                 | Polar Int Int
                 deriving (Show)

-- some bad examples here
getX :: Coordinates -> Int
getX (Cartesian x _) = x
getX _ = -1

inFirstQuad :: Coordinates -> Bool
inFirstQuad (getX -> 1) = True
inFirstQuad (getX -> (-1)) = False
inFirstQuad _ = False
