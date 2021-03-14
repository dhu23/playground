-- Node is constructed with node value, left tree and right tree
data Tree a = Leaf | Node a (Tree a) (Tree a) deriving (Show)

buildTree1 :: Tree Int
buildTree1 = Node 5 
                  (Node 2 (Node 1 Leaf Leaf) (Node 4 Leaf Leaf)) 
                  (Node 3 Leaf (Node 6 Leaf Leaf))

buildTree2 :: Tree Int
buildTree2 = Node 5
                  (Node 2 (Node 1 Leaf (Node 4 Leaf Leaf)) Leaf)
                  (Node 3 Leaf Leaf)


buildTree3 :: Tree Int
buildTree3 = Node 10
                  (Node 5 (Node 1 Leaf (Node 2 Leaf Leaf)) (Node 7 Leaf Leaf))
                  (Node 20 Leaf (Node 25 (Node 23 Leaf Leaf) Leaf))


data TreeBalance = Imbalanced | Balanced Int deriving (Show)


-- Nothing means the tree is not balanced
-- Just a means the tree is balanced and the height is a
calcBalanced :: Maybe Int -> Maybe Int -> Maybe Int
calcBalanced Nothing _ = Nothing
calcBalanced _ Nothing = Nothing
calcBalanced (Just l) (Just r) = 
    if l > r+1 || l < r-1 
    then Nothing
    else Just ((max l r) + 1)

checkBalanced :: Tree a -> Maybe Int
checkBalanced Leaf = Just 0
checkBalanced (Node v l r) = calcBalanced (checkBalanced l) (checkBalanced r)

verifyRange :: Ord a => a -> (Maybe a) -> (Maybe a) -> Bool
verifyRange _ Nothing Nothing = True
verifyRange v Nothing (Just upper) = (v < upper)
verifyRange v (Just lower) Nothing = (lower < v)
verifyRange v (Just lower) (Just upper) = (v < upper) && (lower < v)

validateBST :: Ord a => Tree a -> (Maybe a) -> (Maybe a) -> Bool
validateBST Leaf _ _ = True
validateBST (Node v l r) maybeLower maybeUpper = 
    let goodNode = verifyRange v maybeLower maybeUpper
        goodLeftTree = validateBST l maybeLower (Just v)
        goodRightTree = validateBST r (Just v) maybeUpper
    in goodNode && goodLeftTree && goodRightTree

--type TreeBalance' = (Bool, Int)


main :: IO()
main = do 
    putStrLn $ show $ buildTree1
    putStrLn $ show $ buildTree2
    putStrLn $ show $ buildTree3
    putStrLn $ show $ checkBalanced (buildTree1)
    putStrLn $ show $ checkBalanced (buildTree2)
    putStrLn $ show $ validateBST buildTree1 Nothing Nothing
    putStrLn $ show $ validateBST buildTree2 Nothing Nothing
    putStrLn $ show $ validateBST buildTree3 Nothing Nothing
