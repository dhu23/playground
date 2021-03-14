newtype Cont r a = Cont { runCont :: ((a -> r) -> r) }
-- for example a = Int, r = String
-- show :: Int -> String
-- head . show :: Cont String Int

{-
instance Functor (Cont r) where
    -- fmap :: (a -> b) -> Cont r a -> Cont r b
    -- c :: (a -> r) -> r
    -- c' :: (b -> r) -> r
    -- let k :: b -> r
    fmap f (Cont c) = Cont $ \k -> 
-}

instance Applicative (Cont r) where
    pure a = Cont $ \k -> k a
    -- Cont r (a -> b) <*> Cont r a :: Cont r b
    -- f :: ((a -> b) -> r) -> r
    -- c :: (a -> r) -> r
    -- k :: b -> r
    -- runCout (c a) :: r
    -- runCout (f atob) :: 
    (Cont f) <*> (Cont c) = Cont $ \k -> 

instance Monad (Cont r) where
    -- return :: a -> Cont r a 
    -- a -> (a -> r) -> r
    return a = Cont $ \k -> k a
    -- >>= :: Cont r a -> (a -> Cont r b) -> Cont r b
    -- ((a -> r) -> r) -> (a -> (b -> r) -> r) -> ((b -> r) -> r)
    -- k :: b -> r
    -- c :: (a -> r) -> r
    -- f :: a -> Cont ( (b -> r) -> r )
    -- runCount (f a) :: r
    -- \a -> runCont (f a) k :: a -> r
    (Cont c) >>= f = Cont $ \k -> c (\a -> runCont (f a) k)

