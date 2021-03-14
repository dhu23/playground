import Control.Monad
-- import System.IO


-- IO type constructor is a member of Monad class and the MonadError class

translate :: String -> String -> Char -> Char
translate [] _ c = c
translate (x:xs) [] c = if x == c then ' ' else translate xs [] c
translate (x:xs) [y] c = if x == c then y else translate xs [y] c
translate (x:xs) (y:ys) c = if x == c then y else translate xs ys c

main = do 
    print 5
