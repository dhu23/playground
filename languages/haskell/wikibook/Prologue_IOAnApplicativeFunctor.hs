import Text.Read (readMaybe)

-- main = readData


interactiveDoublubg = do
    input <- getLine
    let r = readMaybe input :: Maybe Double
    case r of 
        Nothing -> do
            putStrLn $ "not a number: " ++ input
            interactiveDoublubg
        Just d -> putStrLn . show $ 2*d

interactiveSumming = do
    inputx <- getLine
    inputy <- getLine
    let mx = readMaybe inputx :: Maybe Double
        my = readMaybe inputy :: Maybe Double
    let tryAgain = do putStrLn $ "bad input:" ++ inputx ++ " " ++ inputy
                      interactiveSumming
    case mx of 
        Nothing -> tryAgain
        Just x -> case my of 
            Nothing -> tryAgain
            Just y -> putStrLn . show $ x+y

interactiveSumming' = do
    inputx <- getLine
    inputy <- getLine
    let mx = readMaybe inputx :: Maybe Double
        my = readMaybe inputy :: Maybe Double

    case (+) <$> mx <*> my of 
        Nothing -> do
            putStrLn $ "bad input:" ++ inputx ++ " " ++ inputy
            interactiveSumming'
        Just s -> putStrLn . show $ s

interactiveConcatenating = do
    putStrLn "Choose two strings:"
    sx <- getLine
    sy <- getLine 
    putStrLn "concatenate them:"
    putStrLn $ sx ++ sy

interactiveConcatenating' = do
    putStrLn "Choose two strings:"
    s <- (++) <$> getLine <*> getLine
    putStrLn $ "concatenate them:" ++ s
