import Data.Char
import Control.Monad

data Parsed = Digit Integer | Hex Integer | Word String deriving Show


parseHexDigit :: Parsed -> Char -> [Parsed]
parseHexDigit (Hex n) c
    | isHexDigit c = return $ Hex $ (n*16) + (toInteger (digitToInt c))
parseHexDigit _ _ = mzero


parseDigit :: Parsed -> Char -> [Parsed]
parseDigit (Digit n) c
    | isDigit c = return $ Digit $ (n*10) + (toInteger (digitToInt c))
parseDigit _ _ = mzero


parseWord :: Parsed -> Char -> [Parsed]
parseWord (Word s) c
    | isAlpha c = return $ Word $ s ++ [c]
parseWord _ _ = mzero

-- matches b -> a -> m b
parse :: Parsed -> Char -> [Parsed]
parse p c = (parseHexDigit p c) `mplus`
            (parseDigit p c) `mplus`
            (parseWord p c)


-- foldM :: (Monad m, Foldable t) => (b -> a -> m b) -> b -> t a -> m b
-- foldM f a [] = return a
-- foldM f a (x:xs) = f a x >>= \y -> foldM f y xs
-- pseudo code illustration
-- foldM f a1 [x1, x2, ..., xn] = do a2 <- f a1 x1
--                                   a3 <- f a2 x2
--                                   ...
--                                   f an xn
parseArg :: String -> [Parsed]
parseArg s = do
    init <- (return $ Hex $ 0) `mplus` 
            (return $ Digit $ 0) `mplus`
            (return $ Word $ "")
    foldM parse init s

main = do
    print $ parseArg "dead"
    print $ parseArg "10"
    print $ parseArg "123d"
    print $ parseArg "d12t"
