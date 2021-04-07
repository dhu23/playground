module HasQ 
    ( qint
    , qchar
    , qbool
    , qsymbol
    , qstring
    , qints
    , qchars
    , qbools
    , qsymbols
    , qenlist
    , qneg
    , qabs
    ) where

import qualified Data.Char as DC
import qualified Control.Monad.Except as CME

data BaseData
    = I (Maybe Int)
    | C (Maybe Char)
    | B (Maybe Bool)
    | S String
    deriving Show

int :: Int -> BaseData
int = I . Just

char :: Char -> BaseData
char = C . Just

bool :: Bool -> BaseData
bool = B . Just

symbol :: String -> BaseData 
symbol = S

nint = I Nothing
nchar = C Nothing
nbool = B Nothing
nsymbol = S []

null :: BaseData -> Bool
null (I Nothing) = True
null (C Nothing) = True
null (B Nothing) = True
null (S []) = True
null _ = False

data QType 
    = QInt | QInts
    | QChar | QChars
    | QBool | QBools
    | QSym | QSyms
    deriving Show 

-- TODO maybe use general traversable instead of List
data Q a 
    = QAtom a 
    | QList [Q a]
    | QDict [(Q BaseData, Q a)]
    deriving Show

qint :: Int -> Q BaseData
qint = QAtom . I. Just

qchar :: Char -> Q BaseData
qchar = QAtom . C . Just

qbool :: Bool -> Q BaseData
qbool = QAtom . B . Just

qsymbol :: String -> Q BaseData
qsymbol = QAtom . S 

qstring :: String -> [Q BaseData]
qstring = fmap qchar

qints = QList . fmap qint

qchars = QList . fmap qchar

qbools = QList . fmap qbool 

qsymbols = QList .  fmap qsymbol

qenlist :: Q a -> Q a
qenlist x = QList [x]

qcount :: Q BaseData -> Int
qcount (QAtom _) = 1
qcount (QList qs) = length qs
qcount (QDict qkvs) = length qkvs 

-- fmap is responsible for monadic function's recursive behavior over
-- Q data structures.
instance Functor Q where
    fmap f (QAtom a) = QAtom $ f a
    fmap f (QList qs) = QList $ fmap (fmap f) qs
    fmap f (QDict kvs) = QDict $ fmap (fmap (fmap f)) kvs

instance Foldable Q where
    foldMap f (QAtom a) = f a
    foldMap f (QList qs) = foldMap (foldMap f) qs
    foldMap f (QDict kvs) = foldMap (snd . fmap (foldMap f)) kvs

instance Traversable Q where
    sequenceA (QAtom fa) = fmap QAtom fa
    sequenceA (QList qfas) = fmap QList $ sequenceA $ fmap sequenceA qfas
    sequenceA (QDict kfvs) = fmap QDict $ sequenceA $ fmap (sequenceA . fmap sequenceA) kfvs
    -- kfvs is of type [(Q BaseData, Q f a)]
    -- each element is of type (Q BaseData, Q f a), which is a functor (,)
    -- sequenceA $ Q f a = f Q a
    -- fmap sequenceA $ (Q BaseData, Q f a) = (Q BaseData, f Q a)
    -- (,) is also a traversable data type
    -- sequenceA $ (Q BaseData, f Q a) = f (Q BaseData, Q a)
    -- this explains (sequenceA . fmap sequenceA) part, the trickiest part

data QErr 
    = TypeErr
    | RankErr
    deriving Show

-- rank 1 atomic functior
type AtomicFunc = BaseData -> Either QErr BaseData

liftAtomic :: AtomicFunc -> Q BaseData -> Either QErr (Q BaseData)
liftAtomic f q = sequenceA (fmap f q)

baseToInt :: BaseData -> Either QErr (Maybe Int)
baseToInt (I mi) = pure mi
baseToInt (C mc) = pure $ fmap DC.ord mc
baseToInt (B mb) = pure $ fmap btoi mb 
    where 
    btoi True = 1
    btoi False = -1
baseToInt (S _) = Left TypeErr

neg :: AtomicFunc
neg = fmap (I . fmap negate) . baseToInt

qneg = liftAtomic neg

abs' :: AtomicFunc
abs' = fmap (I . fmap abs) . baseToInt

qabs = liftAtomic abs'

qcomma :: Q BaseData -> Q BaseData -> Either QErr (Q BaseData)
qcomma q1@(QAtom _) q2@(QAtom _) = Right $ QList [q1, q2]
qcomma (QList qs) q@(QAtom _) = Right $ QList (qs ++ [q])
qcomma q@(QAtom _) (QList qs) = Right $ QList (q:qs)
qcomma _ _ = Left TypeErr

--- rank 2 atomic function

