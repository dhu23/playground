data Tree a = Node a [Tree a] deriving (Show)

instance Functor Tree where
    -- fmap :: (a -> b) -> Tree a -> Tree b
    -- use InstanceSigs to allow this
    fmap f (Node a tas) = Node (f a) $ map (fmap f) tas

--data Either' e a = Left e | Right a deriving (Show)
newtype myEither e = Either e

instance Functor (Either e) where
    fmap _ (Left err) = Left err
    fmap f (Right d) = Right (f d)

main = do 
    let tree1 = Node 1 [(Node 2 []), (Node 3 [Node 4 []])]
    print $ fmap show tree1
    --print $ Right 5
