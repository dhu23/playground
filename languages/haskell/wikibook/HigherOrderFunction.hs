-- challenging: the following exercise combines what you have learned 
-- about higher order functions, recursion and I/O. We are going to 
-- recreate what is known in imperative languages as a for loop. 
-- Implement a function:
--
-- for :: a -> (a -> Bool) -> (a -> a) -> (a -> IO ()) -> IO ()
-- for i p f job = ???/
-- for 1 (< 10) (+1) print 
-- which prints the number 1 to 9 on the screen


for :: a -> (a -> Bool) -> (a -> a) -> (a -> IO ()) -> IO ()
for i p f job 
    | p i = do 
        job i
        for (f i) p f job
    | otherwise = return ()

sequenceIO :: [IO a] -> IO [a]
sequenceIO [] = return []
sequenceIO (io:ios) = do
    x <- io
    xs <- sequenceIO ios
    return (x:xs)

mapIO :: (a -> IO b) -> [a] -> IO [b]
mapIO f xs = sequenceIO $ map f xs


-- write implementations for curry, uncurry and const
curry' :: ((a, b) -> c) -> a -> b -> c
curry' f a b = f (a, b)

uncurry' :: (a -> b -> c) -> (a, b) -> c
uncurry' f (a, b) = f a b

const' :: a -> b -> a
const' a _ = a

-- interesting points
-- uncurry const = fst
-- curry fst = const
-- curry swap :: a -> b -> (b, a)

-- implement foldl with foldr

