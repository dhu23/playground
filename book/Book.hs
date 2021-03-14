module Book where

-- Book for order driven market
-- it contains both bids and asks,
-- 100     20               102     25
--  98     25               104     35
--  97     33               105     10
--  95     40               107     57
--                          109     60

-- for each level, there are multiple order listed, by entry order

data Order = Order 
           { client :: String
           , ord :: String
           , px :: Int
           , qty :: Int
           } deriving Show

data Level = Level 
           { px :: Int
           , orders :: [Order] 
           } deriving Show

{--
import qualified Data.List as DL
import qualified Data.Function as DF

data PQ = PQ { price :: Int
             , quanityt :: Int  
             } deriving (Show)

instance Eq PQ where
    (PQ pa _) == (PQ pb _) = pa == pb

instance Ord PQ where
    compare (PQ pa _) (PQ pb _) = compare pa pb

toPQ :: (Int, Int) -> PQ
toPQ = uncurry PQ

type Quote = PQ
type BookLevel = PQ

type BookLevelOrder = PQ -> PQ -> Ordering

asksOrder :: BookLevelOrder
asksOrder = compare

bidsOrder :: BookLevelOrder
bidsOrder = flip compare

data Levels = BidsLevels [BookLevel] | AsksLevels [BookLevel] deriving Show

getLevels :: Levels -> [BookLevel]
getLevels (BidsLevels lvls) = lvls
getLevels (AsksLevels lvls) = lvls


levelOrder :: Levels -> BookLevelOrder
levelOrder (BidsLevels _) = bidsOrder
levelOrder (AsksLevels _) = asksOrder

data Book = Book { bids :: Levels, asks :: Levels } deriving (Show)

addLevel' :: BookLevelOrder -> BookLevel -> [BookLevel] -> [BookLevel]
addLevel' _ n [] = [n]
addLevel' blOrder pq0@(PQ p0 q0) bls@(pq@(PQ p q):rest)
    | blOrder pq0 pq == LT = pq0:bls
    | blOrder pq0 pq == EQ = (PQ p0 (q0+q)):rest
    | otherwise = pq:(addLevel' blOrder pq0 rest)

makeBookLevels :: [(Int, Int)] -> [BookLevel]
makeBookLevels = map toPQ

addLevel :: BookLevel -> Levels -> Levels
addLevel bl lvls@(BidsLevels bls) = BidsLevels $ addLevel' bidsOrder bl bls
addLevel bl lvls@(AsksLevels bls) = AsksLevels $ addLevel' asksOrder bl bls

fromList :: ([BookLevel] -> Levels) -> [BookLevel] -> Levels
fromList ctr list = foldr addLevel (ctr []) list

testBids = fromList BidsLevels $ map toPQ $ 
           [ (100, 20)
           , (98, 25)
           , (97, 33)
           , (95, 40) 
           ]
testAsks = fromList AsksLevels $ map toPQ $ 
           [ (102, 25)
           , (104, 35)
           , (105, 10)
           , (107, 57)
           , (109, 60)
           ]

data Order = Buy PQ | Sell PQ deriving (Show)

type Trade = PQ

removeLevel' :: BookLevelOrder 
             -> PQ 
             -> [BookLevel] 
             -> (Maybe PQ, [Trade], [BookLevel])
removeLevel' _ pq [] = (Just pq, [], [])
removeLevel' blOrder pq0@(PQ p0 q0) bls@(pq@(PQ p q):rest)
    | blOrder pq0 pq == LT = (Just pq0, [], bls)
    | q0 < q = (Nothing, [PQ p q0], (PQ p (q-q0)):rest) 
    | otherwise = (leftOver, (PQ p q):ts, restLevels)
    where 
    (leftOver, ts, restLevels) = removeLevel' blOrder (PQ p0 (q0-q)) rest

matchBids :: PQ -> Levels -> (Maybe PQ, [Trade], Levels)
matchBids pq bids = (mpq, ts, BidsLevels bls)
    where (mpq, ts, bls) = removeLevel' bidsOrder pq (getLevels bids)

matchAsks:: PQ -> Levels -> (Maybe PQ, [Trade], Levels)
matchAsks pq asks = (mpq, ts, AsksLevels bls)
    where (mpq, ts, bls) = removeLevel' asksOrder pq (getLevels asks)
--}
