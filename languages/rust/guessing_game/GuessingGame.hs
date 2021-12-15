import System.Random (randomRIO)


main :: IO ()
main = do
    secretNumber <- randomRIO (1, 100) :: IO Int
    print secretNumber
    guess secretNumber


guess :: Int -> IO ()
guess n = do
    -- TODO need to figure out how to do safe read
    x <- readLn :: IO Int
    case x `compare` n of
        GT -> putStrLn "Too big!" >> guess n
        LT -> putStrLn "Too small!" >> guess n
        EQ -> do
            putStrLn "You win!"
