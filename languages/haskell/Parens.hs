type Paren = String

genParens :: Int -> [Paren]
genParens 0 = [""]
genParens 1 = ["()"]
genParens n = concat $ map genParens' [0..n-1]
  where
    genParens' i = do
        part1 <- genParens (n-i-1)
        part2 <- genParens i
        return $ "(" ++ part1 ++ ")" ++ part2

