import Control.Monad (guard)
char :: Char -> String -> Maybe (Char, String)
char c s = do
    guard (not $ null s)
    let (c' : s') = s
    if c == c' then Just (c, s') else Nothing

digit :: Int -> String -> Maybe Int
digit _ [] = Nothing
digit i (c:_) | i > 9 || i < 0 = Nothing
              | otherwise = do if [c] == show i then Just i else Nothing

--hexChar :: String -> Maybe (String, String)

