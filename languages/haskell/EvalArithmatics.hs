import Control.Monad.Except
import Control.Monad.State
import Data.Char

-- you are given a string of characters, it should be only just contain
-- 0-9, +, - and *, forming a legal mathmatical expression
-- "3+5" is 8
-- "3*5" is 15
-- "15+2*3" is 21


data Op = Add | Sub | Mul deriving (Eq, Show)
data MathStacks = MathStacks { numbers :: [Int]
                             , operators :: [Op] 
                             , current :: Int }
                             deriving Show

data EvalErr = ParseErr { position :: Int, reason :: String }
             | StackErr String 
             | OpErr String
             deriving Show

collapseOn :: MathStacks -> [Op] -> Either EvalErr MathStacks
collapseOn ms@(MathStacks _ [] _) _ = return ms
collapseOn ms@(MathStacks ns ops _) permittedOps
    | length ns < 2 = Left $ StackErr ("numbers length < 2:" ++ show ns)
    | not $ op `elem` permittedOps = return ms
    | otherwise = do
        let n = calc op n1 n2
        return $ ms { numbers=(n:nrest), operators=oprest }
    where (n2:n1:nrest) = ns
          (op:oprest) = ops

calc :: Op -> Int -> Int -> Int
calc Add n1 n2 = n1 + n2
calc Sub n1 n2 = n1 - n2
calc Mul n1 n2 = n1 * n2

toOp :: Char -> Either EvalErr Op
toOp c
    | c == '+' = return Add
    | c == '-' = return Sub
    | c == '*' = return Mul
    | otherwise = Left $ OpErr $ "unknown op=" ++ [c]

exec :: MathStacks -> Either EvalErr MathStacks
exec ms@(MathStacks ns ops curr)
    | nlen /= olen + 1 = Left $ StackErr ("inconsistent stacks")
    | olen == 0 = Right ms
    | otherwise = do 
        let (n2:n1:nrest) = ns 
            (op:oprest) = ops
            n = calc op n1 n2
        return $ MathStacks (n:nrest) oprest curr
    where nlen = length ns
          olen = length ops

exec' :: MathStacks -> Either EvalErr MathStacks
exec' ms@(MathStacks _ [] _) = return ms
exec' ms = (exec ms) >>= exec'

eval :: MathStacks -> Either EvalErr Int
eval (MathStacks (n:_) [] _) = return n
eval _ = Left $ StackError ("inconsistent stacks")

horner :: Int -> Int -> Int
horner digit num = num * 10 + digit

updateCurr :: Int -> MathStacks -> MathStacks
updateCurr digit ms@(MathStacks _ _ curr) = ms { current=horner digit curr }

updateOps :: Op -> MathStacks -> Either EvalErr MathStacks
updateOps op ms@(MathStacks _ ops _) = return $ ms { operators=(op:ops) }

updateNum :: MathStacks -> MathStacks
updateNum ms@(MathStacks ns _ curr) = ms { numbers=(curr:ns), current=0 }

parse :: (Char, Int) -> MathStacks -> Either EvalErr MathStacks
parse (c, idx) ms@(MathStacks ns ops curr) 
    | c `elem` ['+', '-', '*'] = do
        -- current number run is done
        op <- toOp c
        let ms0 = updateNum ms
        -- if there is existing multiplication on top. collapse it
        ms1 <- collapseOn ms0 [Mul]
        ms2 <- if op == Add || op == Sub 
            -- if there is existing addition or subtraction, do it
            then collapseOn ms1 [Add, Sub]
            else return ms1 
        updateOps op ms2
    | isDigit c = Right $ updateCurr (digitToInt c) ms
    | otherwise = Left $ 
        ParseErr idx ("err char at pos=" ++ show idx ++ " char:" ++ [c])
    where nlen = length ns
          olen = length ops

evalStringE :: String -> Either EvalErr MathStacks
evalStringE s = foldM (flip parse) emptyStack $ zip s [1..]


top :: MathStacks -> Either EvalErr Int
top (MathStacks [] _ _) = StackErr "no value left"
top (MathStacks (n:_) _ _) = return n
    
calcString :: String -> Either EvalErr Int
calcString s = do 
    ms <- evalStringE s -- or use ms <- calcStringE s
    ms' <- exec' $ updateNum ms
    top ms'

emptyStack = MathStacks [] [] 0

main :: IO ()
main = do 
    print $ calcString "13+15"
    print $ calcString "3+25*4"
    print $ calcString "1-2*2*2+7"
