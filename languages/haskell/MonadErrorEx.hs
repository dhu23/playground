import Control.Monad.Except
import Data.Char

-- class Monad m => MonadError e m | m -> e where
--     throwError :: e -> m a
--     catchError :: m a -> (e -> m a) -> m a

data ParseError = Err { location :: Int, reason :: String } deriving Show

type ParseMonad = Either ParseError 

-- parseHexDigit attempts to convert a single hex digit into an Integer 
-- in the ParseMonad monad and throws an error on an invalid character
parseHexDigit :: Char -> Int -> ParseMonad Integer
parseHexDigit c idx = 
    if isHexDigit c 
        then return (toInteger (digitToInt c))
        else throwError (Err idx ("Invalid character'"++[c]++"'"))

-- parseHex parses a string containing a hexdecimal number into 
-- an Integer in the ParseMonad monad. A prase error from parseHexDigit
-- will cause an exceptional return from parseHex
parseHex :: String -> ParseMonad Integer
parseHex s = go s 0 1
    where
    go [] val _ = return val
    go (c:cs) val idx = do
        d <- parseHexDigit c idx
        go cs (val*16+d) (idx+1)

-- converts an Integer into a String in the ParseMonad monad
toString :: Integer -> ParseMonad String
toString = return . show

-- takes a String containing a hexadecimal representation of a number
-- to a String containing a decimal representation of that number. A parse
-- error on the input String will generate a descriptive error message as
-- the output String
convert :: String -> String 
convert s = case res of 
    Left e -> "At index " ++ (show . location $ e) ++ ":" ++ (reason e)
    Right val -> val
    where 
    res = do -- in the context of ParseMonad (or Either ParseError)
        n <- parseHex s -- :: ParseMonad Integer
        toString n

-- see ListMonadExample.hs for a similar problem
