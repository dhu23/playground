data BinaryTree a = Leaf | Node (BinaryTree a) a (BinaryTree a) deriving Show


buildBinaryTree1 :: BinaryTree Int
buildBinaryTree1 = let bottom1 = Node Leaf (-1) Leaf
                       bottom2 = Node Leaf (-2) Leaf
                       bottom3 = Node Leaf 29 Leaf
                       bottom4 = Node Leaf 4 Leaf
                       left = Node bottom1 15 bottom2
                       right = Node bottom3 (-2) bottom4
                   in Node left 10 right 

-- node val, left sum, right sum
binaryTreeTraverse :: (BinaryTree Int) -> (Int, Int, Int)
binaryTreeTraverse Leaf = (0, 0, 0)
binaryTreeTraverse (Node left val right) = 
    let (leftVal, leftLeafSum, leftPathSum) = binaryTreeTraverse left
        (rightVal, rightLeafSum, rightPathSum) = binaryTreeTraverse right
    in (val
       ,foldl1 max [leftLeafSum+val, rightLeafSum+val, val]
       ,leftLeafSum+val+rightLeafSum)

-- max path sum, max leaf sum
maxPathValue :: (BinaryTree Int) -> (Int, Int)
maxPathValue Leaf = (0, 0)
maxPathValue (Node l v r) = 
    let (leftPathSumMax, leftLeafSumMax) = maxPathValue l
        (rightPathSumMax, rightLeafSumMax) = maxPathValue r
        maxSubPathSum = max leftPathSumMax rightPathSumMax
        currentPathSumMax = v+(max 0 leftLeafSumMax)+(max 0 rightLeafSumMax)
        maxPathSum = max maxSubPathSum currentPathSumMax
    in (maxPathSum, (max leftLeafSumMax rightLeafSumMax)+v)
        

main :: IO()
main = do 
    putStrLn $ show $ binaryTreeTraverse buildBinaryTree1
    putStrLn $ show $ maxPathValue buildBinaryTree1
