import Data.List (sortBy, intercalate)
import Control.Monad (forM_)

data Tower = Tower {stack :: [Int], idx :: Int} deriving (Show)

cmpTower :: Tower -> Tower -> Ordering
cmpTower t1 t2 = (idx t1) `compare` (idx t2)

orderTowers :: (Tower, Tower, Tower) -> (Tower, Tower, Tower)
orderTowers (t1, t2, t3) = toTuple $ sortBy cmpTower $ [t1, t2, t3]
    where toTuple [t1, t2, t3] = (t1, t2, t3) 

type History = [(Tower, Tower, Tower)]

showHistory :: History -> String
showHistory = intercalate "\n" . map (showH . orderTowers) . reverse
    where showH (t1, t2, t3) = show $ [stack t1, stack t2, stack t3]

-- looks like some sort of monadic function candidate
-- type HanoiState = State (Tower, Tower, Tower) History
-- move :: Int -> HanoiState
-- move n = do 
--     hist@(h:hs) <- get
--     
move :: Int -> History -> (Tower, Tower, Tower)
        -> ((Tower, Tower, Tower), History)
move n hist (from, buffer, to)
    | n == 1 = case stack from of
                [] -> error (showHistory hist)
                (f:fs) -> let next = ( from {stack=fs}
                                     , buffer 
                                     , to {stack=(f:(stack to))} )
                                  in (next, (next:hist))

    | n > 1 = let ((from1, to1, buffer1), hist1) = 
                            move (n-1) hist (from, to, buffer)
                  ((from2, buffer2, to2), hist2) = 
                            move 1 hist1 (from1, buffer1, to1)
                  ((buffer3, from3, to3), hist3) = 
                            move (n-1) hist2 (buffer2, from2, to2)
              in ((from3, buffer3, to3), hist3)

------------------- test cases ---------------------

hanoiTower :: Int -> IO ()
hanoiTower n = do 
    putStrLn ("doing hanoi tower of " ++ show n)
    let first = Tower [1..n] 1; second = Tower [] 2; third = Tower [] 3
        (last, hist) = move n [(first, second, third)] (first, second, third)
    putStrLn "last one is"
    print $ last
    putStrLn "history is"
    putStrLn $ showHistory hist
    putStrLn $ "total "++show (length hist)++" steps"

test2 :: IO ()
test2 = forM_ [1..5] $ \i -> hanoiTower i

test1 :: IO ()
test1 = do
    let first = Tower [1, 2, 3] 1
        second = Tower [] 2
        third = Tower [] 3
    print $ move 2 [(first, second, third)] (first, third, second)

main :: IO ()
main = test2
