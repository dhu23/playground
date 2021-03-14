-- clone sheep don't have parents
import Control.Monad
import Data.Function

data Sheep = Natural { m :: Sheep, f :: Sheep } | Cloned Int


father :: Sheep -> Maybe Sheep
father (Natural mom dad) = Just dad
father (Cloned _) = Nothing

mother :: Sheep -> Maybe Sheep
mother (Natural mom dad) = Just mom
mother (Cloned _) = Nothing

comb :: Maybe a -> (a -> Maybe b) -> Maybe b
comb Nothing _ = Nothing
comb (Just x) f = f x

maternalPaternalGrandfather0 :: Sheep -> Maybe Sheep
maternalPaternalGrandfather0 s = do 
    m <- mother s
    gf <- father m
    father gf
    -- (Just s) `comb` mother `comb` father `comb` father

-- rewrite maternal grand father, fathers maternal grandmother and
-- mothers paternal grand father functions using return and >>=


maternalGrandfather :: Sheep -> Maybe Sheep
maternalGrandfather s = father =<< mother =<< return s

fathersMaternalGrandmother :: Sheep -> Maybe Sheep 
fathersMaternalGrandmother s = maternalGrandfather =<< father =<< return s

maternalPaternalGrandfather :: Sheep -> Maybe Sheep
maternalPaternalGrandfather s = 
    (return s) >>= mother >>= father >>= father


-- write functions parent and grandparent with signature Sheep -> Maybe Sheep
-- they should return on sheep selected from all sheep matching the description
-- or nothing if there is no such sheep. hint: mplus

parent :: Sheep -> Maybe Sheep
parent s = father s `mplus` mother s

grandparent :: Sheep -> Maybe Sheep
grandparent s = (m >>= mother) `mplus` 
                (m >>= father) `mplus` 
                (p >>= father) `mplus` 
                (p >>= mother) 
    where m = mother s
          p = father s

-- write functions parents and grandparents with Sheep -> [Sheep].
-- they should return all sheep matching the description or empty list if not
maybeToList :: Maybe a -> [a]
maybeToList Nothing = []
maybeToList (Just x) = [x]


parents :: Sheep -> [Sheep]
parents s = pf (mother s) (father s)
    where pf = mplus `on` maybeToList

grandparents :: Sheep -> [Sheep]
grandparents s = (maybeToList $ m >>= mother) `mplus`
                 (maybeToList $ m >>= father) `mplus`
                 (maybeToList $ p >>= father) `mplus`
                 (maybeToList $ p >>= mother)
    where m = mother s
          p = father s

grandparents0 :: Sheep -> [Sheep]
grandparents0 s = parents s >>= parents

-- write functions parent and grandparent with signature (MonadPlus m)
-- They should be useful in both the Maybe and List monads.
-- How does the functions' behavior differ when used with List and Maybe

maybeToMonad :: MonadPlus m => Maybe a -> m a
maybeToMonad Nothing = mzero
maybeToMonad (Just x) = return x 

parentM :: (MonadPlus m) => Sheep -> m Sheep
parentM s = (maybeToMonad $ father s) `mplus` (maybeToMonad $ mother s)

grandparentsM :: (MonadPlus m) => Sheep -> m Sheep
grandparentsM s = parentM s >>= parentM


main = do print 5
