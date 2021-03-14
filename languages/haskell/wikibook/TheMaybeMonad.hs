natural :: Int -> IO (Maybe Int)
natural x | x > 0 = pure (Just x)
          | otherwise = pure Nothing


main = do
    Just x <- natural (-1)
    print x
