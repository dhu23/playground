-- copied down example code from Haskell wiki
-- this is a weird example and gets less interesting as it goes...


newtype Lover a = Lover { loverDiary :: (Name, a) } deriving Show
type Name = String

createLover name times = Lover (name, times)
startAffairWith name (Lover (names, times)) = Lover (name, 0)


jenny = startAffairWith "Jenny "
luisa = startAffairWith "Luisa "
antonia = startAffairWith "Antonia "


bob = createLover "Paula " 5

oneMoreTime (Lover (name, times)) = Lover (name, times+1)

-- oneMoreTime bob
-- oneMoreTime (antonia bob)

changeBeloved newname (Lover (name, times)) = Lover (name ++ newname, times)

chainAffairs (Lover (names, oldtimes)) (Lover (newlady, newtimes)) = 
    Lover (newlady++names, newtimes+oldtimes)

-- now this behaves sorta like a functor map
times f (Lover (name, times)) = Lover (name, f times) 

class Macho f where
    chain :: (Num a) => f a -> f a -> f a

instance Macho Lover where
    chain mychicks = chainAffairs mychicks

-- chain $ (times (+3) (antonia bob))   -- 3
--         (chain $ (times (*2) (changeBeloved "Carla " bob)) -- 10
--                  (times (+2) (luisa bob))) -- 2

instance Functor Lover where
    fmap f = times f

-- behavior of chaining, monadic interface
tellLover newtimes oldtimes = Lover ("", newtimes+oldtimes)
askLover lover answer = Lover (oldnames ++ newname, newtimes)
    where (oldnames, oldtimes) = loverDiary lover
          (newname, newtimes) = loverDiary (answer oldtimes)
          
tellMyself newtimes = Lover ("", newtimes)
newLove love = Lover (love, 0)
