-- predict what should be the result of ["bunny", "rabbit"] >>= generation

generation :: a -> [a]
generation = replicate 3

themselvesTimes :: [Int] -> [Int] -- takes each n and generate n copies
themselvesTimes ns = ns >>= (\ n -> replicate n n)

-- All monads have an instance of Applicative. In particular (<*>) for that
-- instance might be defined as 
-- fs <*> xs = concatMap (\ f -> map f xs) fs
-- map each f on xs, which generates a list and finally concat them
fs <*> xs = [ f x | f <- fs, x <- xs]

