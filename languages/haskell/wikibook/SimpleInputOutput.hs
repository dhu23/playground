main = do
    name <- getLine
    if name `elem` ["Simon", "John", "Phil"]
        then putStrLn "Haskell is a great programming language"
        else 
            if name == "Koen"
                then putStrLn "Haskell is fun"
                else putStrLn "I don't know you"
