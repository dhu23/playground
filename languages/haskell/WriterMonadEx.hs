import Control.Monad.Trans.Writer
import Control.Monad.Trans.Reader
import Control.Monad.Trans.Class
import Control.Monad
import Control.Monad.Identity (Identity)

import Data.Monoid
import Data.Char (toLower)

data Person 
    = Person 
    { pName :: String
    , pAge :: Int
    }

type PersonDB = [Person]

alex = Person "Alex" 25
john = Person "John" 36
becky = Person "Becky" 31

verifyPerson :: String -> Reader PersonDB Bool
verifyPerson name = do
    db <- ask
    return $ name `elem` (map pName db)
    
fromReader :: Monad m => Reader r a -> ReaderT r m a
fromReader = reader . runReader

type Log = String

verifyPerson' :: String -> WriterT Log (Reader PersonDB) Bool
verifyPerson' name = do
    tell $ "checking if " ++ name ++ " is in db\n"
    ret <- lift $ verifyPerson name
    if ret 
       then tell $ "found " ++ name ++ "\n"
       else tell $ "failed to find " ++ name ++ "\n"
    return ret

logger :: (MonadTrans t, Monad m) => String -> t (WriterT Log m) ()
logger = lift . tell

verifyPerson'' :: String -> ReaderT PersonDB (Writer Log) Bool
verifyPerson'' name = do
    logger $ "checking if " ++ name ++ " is in db\n"
    ret <- fromReader $ verifyPerson name
    if ret 
       then logger $ "found " ++ name ++ "\n"
       else logger $ "failed to find " ++ name ++ "\n"
    return ret
        

checkNames = runReader . runWriterT . mapM verifyPerson'

test = do 
    let (bs, out) = checkNames ["John", "Joe", "Mary"] [alex, john, becky]
    print bs
    putStrLn out

test2 = do
    let (bs, out) = runWriter $ runReaderT ( mapM verifyPerson'' ["John"] ) [alex]
    print bs
    putStrLn out


------------ test out pass function in writer


pushBack :: Char -> Writer String () -> Writer String ()
pushBack c = (>> tell [c])


modFront :: (Char -> Char) -> Writer String () -> Writer String ()
modFront f m = pass $ do
    ((), w) <- listen m
    case w of -- if pass here, it `mappend` a modified new String
      [] -> return ((), id)
      (x:xs) -> return ((), \_ -> (f x):xs)

-- pass :: (Monoid w, Monad m) => WriterT w m (a, w -> w) -> WriterT w m a
deleteOn :: (Monoid w) => (w -> Bool) -> Writer w a -> Writer w a
deleteOn p m = pass $ do
    (a, w) <- listen m
    if p w
       then return (a, id)
       else return (a, const mempty)

p = (>5) . length . head 

logTwo :: Writer [String] ()
logTwo = do
    deleteOn p $ tell ["foo"] -- no logging
    deleteOn p $ tell ["foobar"]
    deleteOn p $ tell ["foo"] -- no logging
    deleteOn p $ tell ["foo", "barfoo"] -- no logging
    deleteOn p $ tell ["barfoo", "foo"]
