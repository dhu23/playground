import System.Random (randomRIO, randomR, StdGen, mkStdGen)
import Control.Monad (liftM, forM, replicateM)
import Control.Monad.State (State, get, put, evalState)
import Data.Map (Map, adjust, empty, member, insert)

---------------------- State based random functions ------------------------
randBit :: State StdGen Int
randBit = do 
    g <- get
    let (i, g') = randomR (0, 1) g
    put g'
    return i

multi :: Int -> State StdGen a -> State StdGen [a]
multi = replicateM

randInt :: Int -> State StdGen Int
randInt u = uniform' (numOfBits u)
  where
    uniform' n = do
        ret <- liftM horner $ multi n randBit
        if ret >= u
           then uniform' n 
           else return ret

sample :: [a] -> State StdGen a
sample xs = do 
    n <- randInt (length xs)
    return (xs !! n)

randDigit = randInt 10

data Weekday = Sunday | Monday | Tuesday | Wednesday 
             | Thursday | Friday | Saturday 
             deriving (Show, Enum, Eq, Ord)

randWeekDay :: State StdGen Weekday
randWeekDay = do
    x <- randInt 7
    return (toEnum x)

upperCaseAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
lowerCaseAlphabet = "abcdefghijklmnopqrstuvwxzy"
digits = "0123456789"

type OrdId = String -- 12 character

randOrdId :: State StdGen OrdId
randOrdId = multi 12 (sample (upperCaseAlphabet ++ digits))

type Quantity = Int
type Price = Double
data FillInfo = FillInfo { fillQ :: Quantity, cumQ :: Quantity, px :: Price }
    deriving Show

data ExchMsg 
    = Ack OrdId
    | Reject OrdId
    | Out OrdId
    | Fill OrdId FillInfo
    deriving Show

data UserAct
    = NewOrd OrdId Quantity Price
    | RplOrd OrdId (Maybe Quantity) (Maybe Price)
    | CxlOrd OrdId

data OrdQty = OrdQty { filled :: Quantity, unfilled :: Quantity } 
    deriving Show

data OrdInfo = OrdInfo { qty :: OrdQty, px :: Price } deriving Show

data Order
    = Init OrdId OrdInfo
    | AtEx OrdId OrdInfo
    | Done OrdId OrdInfo
    | Outd OrdId OrdInfo


------------------------- utility functions --------------------------------
horner = foldl run 0 where run acc n = 2*acc+n

numOfBits n
    | n <= 1 = 1
    | otherwise = let (q, r) = n `divMod` 2 in 1+(numOfBits q)

count :: Ord a => [a] -> Data.Map.Map a Int
count = foldr step empty
  where 
    step n m = case n `member` m of
                 True -> adjust (+1) n m
                 otherwise -> insert n 1 m

main = do 
    summarize (multi 100000 randDigit)
    summarize (multi 100000 randWeekDay)
    summarize (multi 10 randOrdId)
      where summarize m = print $ count $ evalState m $ mkStdGen 0


