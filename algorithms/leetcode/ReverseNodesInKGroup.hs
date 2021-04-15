-- this is the same as built-in list type
data LinkedList' a = EmptyList | Node a (LinkedList' a) deriving (Show)

buildSimpleLinkedList :: Int -> (LinkedList' Int)
buildSimpleLinkedList i  
    | i <= 0 = EmptyList
    | otherwise = Node i (buildSimpleLinkedList (i-1))


-- just using the built-in list instead of customized list
reverseNodesInGroup :: [a] -> Int -> [a]
reverseNodesInGroup [] _ = []
reverseNodesInGroup ns 1 = ns
reverseNodesInGroup ns k = 
    let (ks, rest) = splitAt k ns
    in (reverse ks) ++ (reverseNodesInGroup rest k)


main :: IO()
main = do 
    putStrLn $ show $ buildSimpleLinkedList 5
    putStrLn $ show $ reverseNodesInGroup [1..10] 3
