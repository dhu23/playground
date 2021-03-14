{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}

-- Identity is a trivial functor, defined in Data.Functor.Identity as
-- newtype Identity a = Identity { runIdentity :: a }
-- It has the following Monad instance:
--
-- instance Monad Identity where
--     return a = Identity a
--     m >>= k = k (runIdentity m)
--
-- implement a monad transformer IdentityT, analogous to Identity but 
-- wrapping value of type m a rather than a. Write at least its Monad
-- and MonadTrans instances


newtype Identity a = Identity { runIdentity :: a }

instance Functor Identity where
    fmap f a = Identity $ f (runIdentity a)

instance Applicative Identity where
    pure = Identity
    -- f (a -> b) -> f a -> f b
    -- or Identity (a -> b) -> Identity a -> Identity b
    k <*> x = Identity $ runIdentity k (runIdentity x)

instance Monad Identity where
    return = Identity
    p >>= k = k (runIdentity p)

newtype IdentityT m a = IdentityT { runIdentityT :: m (Identity a) }

instance Functor m => Functor (IdentityT m) where
    -- fmap :: (a -> b) -> IdentityT m a -> IdentityT m b
    -- f :: (a -> b)
    -- runIdentityT :: IdentityT m a -> m (Identity a)
    -- lift f to Identity
    -- fmap f :: Identity a -> Identity b
    -- lift this to m
    -- fmap (fmap f) :: m (Identity a) -> m (Identity b)
    fmap f = IdentityT . fmap (fmap f) . runIdentityT

{--
k1 :: (Applicative m) => IdentityT m (a -> b)
k1 = undefined

x1 :: (Applicative m) => IdentityT m a
x1 = undefined
--}

instance Applicative m => Applicative (IdentityT m) where
    -- pure :: a -> Identity m a
    pure = IdentityT . pure . Identity
    -- (<*>) :: f (a -> b) -> f a -> f b
    -- that is IdentityT m (a -> b) -> IdentityT m a -> IdentityT m b
    -- which is m (Identity (a -> b)) -> m (Identity a) -> m (Identity b)
    -- without m, it is Identity (a -> b) -> Identity a -> Identity b
    -- which is exactly <*> of Identity Applicative
    k <*> x = IdentityT $ (<*>) <$> (runIdentityT k) <*> (runIdentityT x) 

instance Monad m => Monad (IdentityT m) where
    return = IdentityT . return . Identity
    -- (>>=) :: IdentityT m a -> (a -> IdentityT m b) -> IdentityT m b
    -- which is really 
    -- m (Identity a) -> (a -> m (Identity b)) -> m (Identity b)
    x >>= f = IdentityT $ do -- in context of m monad
        Identity xa <- runIdentityT x -- xa :: a
        runIdentityT $ f xa

class MonadTrans t where
    lift :: (Monad m) => m a -> t m a

instance MonadTrans IdentityT where
    lift = IdentityT . fmap Identity 

-- reinvent the wheel for StateT
newtype StateT s m a = StateT { runStateT :: (s -> m (a, s)) }

instance Functor m => Functor (StateT s m) where
    -- fmap :: (a -> b) -> StateT s m a -> StateT s m b
    -- which is (a -> b) -> (s -> m (a, s)) -> (s -> m (b, s))
    f `fmap` (StateT x) = StateT $ \ s -> fmap run (x s)
        where run (a, s) = (f a, s)

instance Monad m => Applicative (StateT s m) where
    -- pure :: a -> StateT s m a
    pure a = StateT $ \ s -> pure (a, s)
    -- <*> :: f (a -> b) -> f a -> f b
    -- which is StateT s m (a -> b) -> StateT s m a -> State s m b
    -- with runStateT, it is 
    -- (s -> m ((a -> b, s)) -> (s -> m (a, s)) -> (s -> m (b, s))
    -- build m (b, s) for the return value for the lambda expression
    -- runStateT k s :: m ((a -> b), s)
    -- runStateT x s1 :: m (a, s)
    -- we need to get m (b, s)
    -- without m context, it is ((a -> b), s) -> (a, s) -> (b, s)
    k <*> x = StateT $ \ s -> do

        -- runStateT k s :: m ((a -> b), s)
        -- m s -> m ((a -> b), s)

        (f, s1) <- runStateT k s -- :: m ((a -> b), s)
        (a, s2) <- runStateT x s1
        return (f a, s2)

instance (Monad m) => Monad (StateT s m) where
    return a = StateT $ \ s -> return (a, s)
    -- >>= :: StateT s m a -> (a -> StateT s m b) -> StateT s m b
    (StateT x) >>= f = StateT $ \ s -> do
        (v, s') <- x s
        runStateT (f v) s'

-- create MonadState class
class Monad m => MonadState s m | m -> s where
    get :: m s
    get = state $ \s -> (s, s)

    put :: s -> m ()
    put s = state $ \_ -> ((), s)

    state :: (s -> (a, s)) -> m a
    state f = do
        s <- get
        let (a, s') = f s
        put s'
        return a
    

-- are MaybeT (State s) and StateT s Maybe equivalent
-- MaybeT (State s) a ---> wrapped around State s (Maybe a)
-- which is essentially s -> (Maybe a, s)
-- StateT s Maybe a ---> wrapped around s -> Maybe (a, s)




----------------- writer monad ---------------------
newtype Writer w a = Writer { runWriter :: (a, w) }
instance Functor (Writer w) where
    fmap f m = Writer (f a, w)
        where (a, w) = runWriter m

instance (Monoid w) => Applicative (Writer w) where
    pure a = Writer (a, mempty)
    -- Writer w (a -> b) -> Writer w a -> Writer w b
    -- runWriter f :: ((a -> b), w)
    -- runWriter x :: (a, w)
    (Writer (f, w1)) <*> (Writer (x, w2)) = Writer (f x, w1 `mappend` w2) 

instance (Monoid w) => Monad (Writer w) where
    return = pure
    -- Writer w a -> (a -> Writer w b) -> Writer w b
    (Writer (a, w)) >>= f = Writer (b, w `mappend` w')
        where (b, w') = runWriter $ f a


newtype Reader e a = Reader { runReader :: (e -> a) }

instance Functor (Reader e) where
    -- fmap :: (a -> b) -> Reader e a -> Reader e b
    -- that is (a -> b) -> (e -> a) -> (e -> b)
    fmap f m = Reader $ f . runReader m

instance Applicative (Reader e) where
    -- pure :: a -> Reader e a 
    pure a = Reader $ const a
    -- f <*> x 
    -- f :: Reader e (a -> b), x :: Reader e a
    (Reader f) <*> (Reader a) = Reader $ \e -> f e (a e)

instance Monad (Reader e) where 
    return = pure
    -- >>= :: (Reader e a) -> (a -> Reader e b) -> Reader e b
    (Reader a) >>= f = Reader $ \e -> runReader (f (a e)) e


