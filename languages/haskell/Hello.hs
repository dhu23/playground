import System.Environment

main = do 
    args <- getArgs
    print args
    putStrLn $ "Hello, " ++ args !! 0
