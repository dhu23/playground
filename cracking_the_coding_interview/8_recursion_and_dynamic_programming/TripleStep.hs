import Control.Monad (forM_) 
import qualified Data.Map as M

naiveSteps :: [Integer] -> Integer -> Integer
naiveSteps steps n
    | n < 0 = 0
    | n == 0 = 1
    | otherwise = sum $ map (naiveSteps steps) (map (n-) steps)

naiveTripleSteps = naiveSteps [1, 2, 3]

type Memory = M.Map Integer Integer

memorizationSteps :: [Integer] -> Integer -> Memory -> (Integer, Memory)
memorizationSteps steps n mem 
    | n < 0 = (0, mem)
    | n == 0 = (1, mem)
    | otherwise = case M.lookup n mem of
        Just x -> (x, mem)
        Nothing -> let moves = map (n-) steps
                       -- subCalc i = memorizationSteps steps i mem
                       -- we will have to collect the updated memory from 
                       -- each subCalc call. Otherwise it is as inefficient 
                       -- as the naive solution
                       foldStep i (t, m) = 
                            let (t', m') = memorizationSteps steps i m
                            in ((t'+t), m')
                       (total, newMem) = foldr foldStep (0, mem) moves
                   in (total, M.insert n total newMem)

-- I feel like there is something I can do using state Monad 
data MemoryState a = MemoryState {runMemory :: Memory -> (a, Memory)}

memorizationTripeSteps = \n -> memorizationSteps [1, 2, 3] n M.empty

main :: IO ()
main = do 
    forM_ [1..30] $ \n -> do print $ (n, naiveTripleSteps n)
    forM_ [1..30] $ \n -> do print $ (n, memorizationTripeSteps n)
