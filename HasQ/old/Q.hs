module Q where

import qualified Data.Char as DC
import qualified Control.Monad.Except as CME

data AtomInt = I Int | C Char | B Bool deriving Show
data Atom = AtomInt | Symbol String deriving Show
data Q a = S a | L [Q a] | D [(Q Atom, Q a)] deriving Show

-- fmap is responsible for monadic function's recursive behavior over
-- Q data structures.
instance Functor Q where
    -- f :: (a -> b)
    -- fmap :: (a -> b) -> Q a -> Q b
    fmap f (S a) = S $ f a
    fmap f (L ql) = L $ fmap (fmap f) ql
    fmap f (D kvs) = D $ fmap (\ (k, v) -> (k, fmap f v)) kvs

instance Foldable Q where
    -- foldMap :: Monoid m => (a -> m) -> Q a -> m
    foldMap f (S a) = f a
    foldMap f (L qls) = mconcat $ map (foldMap f) qls
    foldMap f (D kvs) = mconcat $ map (\ (k, v) -> foldMap f v) kvs

instance Traversable Q where
    -- sequenceA :: Q (f a) -> f (Q a)
    sequenceA (S a) = fmap S a -- apply S within the functor context
    sequenceA (L []) = pure $ L []
    -- sequenceA (L qs) :: Q (f a) -> f (Q a)
    -- sequenceA q :: Q (f a) -> f (Q a)
    sequenceA (L (q:qs)) = concatL <$> sequenceA q <*> sequenceA (L qs)
        where 
            concatL :: Q a -> Q a -> Q a
            concatL (L as) (L bs) = L (as ++ bs)
            concatL (L as) qb = L (as ++ [qb])
            concatL qa (L bs) = L (qa:bs)
            concatL qa qb = L [qa, qb]
            
    sequenceA (D []) = pure $ D []
    -- D kvs :: D [(Q Atom, Q (f a))]
    -- sequenceA (D kvs) :: f (Q a)
    sequenceA (D ((k, v):kvs)) = concatD <$> left <*> sequenceA (D kvs)
        where
            -- k :: Q Atom
            -- pure k :: f (Q Atom)
            -- v :: Q (f a)
            -- sequenceA v :: f (Q a)
            -- left :: f (Q Atom, Q a)
            left = (,) <$> pure k <*> sequenceA v
            
            concatD :: (Q Atom, Q a) -> Q a -> Q a
            concatD kv (D kvs) = D (kv:kvs) -- ill pattern match
            

data QErr = TypeErr String deriving Show


aIntToStr :: AtomInt -> String
aIntToStr (I x) = show x
aIntToStr (C c) = [c]
aIntToStr (B b) = show b

toInt :: AtomInt -> Int
toInt (B True) = 1
toInt (B False) = 0
toInt (I x) = x
toInt (C c) = DC.ord c

toAtomInt = I . toInt

liftInt :: (Int -> Int) -> AtomInt -> AtomInt
liftInt f atom = let x = toInt atom in I (f x)

liftInt2 :: (Int -> Int -> Int) -> AtomInt -> AtomInt -> AtomInt
liftInt2 f a b = 
    let x = toInt a 
        y = toInt b
    in I (f x y)

qAdd = liftInt2 (+)
qSub = liftInt2 (-)
qMul = liftInt2 (*)

neg = liftInt negate

-- simulate simple list in Q
toIntQ = S . I
toCharQ = S . C
toBoolQ = S . B
toSymbolQ = S . Symbol

toSimpleQList f = L . map f

toQ1 f = L . map f
toQ2 f = toQ1 (toQ1 f)

toIntQ1 = toQ1 toIntQ
toIntQ2 = toQ2 toIntQ


