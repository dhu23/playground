{-# LANGUAGE OverloadedStrings #-}

module MonadTransEx where

import Data.Text

import qualified Data.Text.IO as T
import Data.Map as Map
import Control.Applicative

data LoginError = InvalidEmail | NoSuchUser | WrongPassword
    deriving Show


getDomain :: Text -> Either LoginError Text
getDomain email = 
    case splitOn "@" email of 
        [name, domain] -> Right domain
        _ -> Left InvalidEmail

{-
printResult' :: Either LoginError Text -> IO ()
printResult' domain = 
    case domain of 
        Right text -> T.putStrLn (append "Domain: " text)
        Left InvalidEmail -> T.putStrLn "ERROR: Invalid domain"
printResult :: Either LoginError Text -> IO ()
printResult = 
    T.putStrLn . either 
        (const "ERROR: Invalid domain")
        (append "Domain: ")
-}

printResult :: Either LoginError Text -> IO ()
printResult res = 
    T.putStrLn $ case res of 
        Right domain -> append "Logged in with domain:" domain
        Left InvalidEmail -> "Invalid email address entered."
        Left NoSuchUser -> "No user with that email exists."
        Left WrongPassword -> "Wrong password."


getToken' :: IO (Either LoginError Text)
getToken' = do -- within IO monad
    T.putStrLn "Enter email address: " -- :: IO ()
    email <- T.getLine -- :: IO Text
    return (getDomain email) :: IO (Either LoginError Text)


-- to make the new function to work with EitherIO LoginError Text
-- we need to accomplish the following 3 conversion
-- 1. IO () -> EitherIO LoginError ()
-- 2. IO Text -> EitherIO LoginError Text
-- 3. IO (Either LoginError Text) -> EitherIO LoginError Text
-- 3. or Either LoginError Text -> EitherIO LoginError Text
-- 1, 2 is generalized to IO a -> EitherIO LoginError a
-- 3 is the constructor or Either e a -> EitherIO LoginError a

wrap :: IO (Either e a) -> EitherIO e a
wrap = EitherIO

liftEither :: Either e a -> EitherIO e a
liftEither = EitherIO . return

liftIO :: IO a -> EitherIO e a
liftIO = EitherIO . fmap Right 

throwE :: e -> EitherIO e a
throwE = liftEither . Left 

-- My Note:
-- a catch is basically a exception, or Either processor in IO context
-- if you have type a result, it is not an exception and just pass through
-- otherwise, there is a chance to handle it, meaning either pass through
-- or turn it into a Right value
catchE :: EitherIO e a -> (e -> EitherIO e a) -> EitherIO e a
catchE throwing handler = EitherIO $ do
    result <- runEitherIO throwing
    case result of
        Left failure -> runEitherIO $ handler failure
        success -> return success


getToken :: EitherIO LoginError Text
getToken = do
    liftIO $ T.putStrLn "Enter email address: "
    input <- liftIO T.getLine
    liftEither (getDomain input)

users :: Map.Map Text Text
users = Map.fromList 
    [ ("example.com", "qwerty123")
    , ("localhost", "password") 
    ]


userLogin' :: IO (Either LoginError Text)
userLogin' = do -- IO monad
    token <- getToken' -- Either LoginError Text

    case token of
        Right domain -> 
            case Map.lookup domain users of 
                Just userpw -> do -- IO moand
                    T.putStrLn "Enter password:"
                    password <- T.getLine

                    if userpw == password
                        then return token
                        else return (Left WrongPassword)
                Nothing -> return (Left NoSuchUser)
        left -> return left


userLogin :: EitherIO LoginError Text
userLogin = do
    domain <- getToken -- EitherIO LoginError Text, domain :: Text
    -- return :: Text -> EitherIO LoginError Text
    -- maybe :: EitherIO LoginError Text 
    --       -> (Text -> EitherIO LoginError Text)
    --       -> Maybe Text 
    --       -> EitherIO LoginError Text
    userpw <- maybe 
        (throwE NoSuchUser) -- EitherIO 
        return -- (Text -> EitherIO)
        (Map.lookup domain users) -- Maybe Text
    password <- liftIO (
        T.putStrLn "Enter password:" >>
        T.getLine
        )
    if userpw == password 
        then return domain
        else throwE WrongPassword
    

userLoginRetryOnWrongPassword :: EitherIO LoginError Text
userLoginRetryOnWrongPassword = do
    domain <- getToken
    userpw <- maybe 
        (throwE NoSuchUser)
        return
        (Map.lookup domain users)

    let checkPassword = do
        password <- liftIO (
            T.putStrLn "Enter password." >> 
            T.getLine
            )
        if userpw == password
            then return domain
            else checkPassword
    
    checkPassword

wrongPasswordHandler :: LoginError -> EitherIO LoginError Text
wrongPasswordHandler WrongPassword = do
    liftIO (T.putStrLn "Wrong password, one more chance.")
    userLogin
wrongPasswordHandler err = throwE err

printError :: LoginError -> EitherIO LoginError a 
printError err = do
    liftIO . T.putStrLn $ case err of
        WrongPassword -> "Wrong password. No more chances."
        NoSuchUser -> "No user with that email exists."
        InvalidEmail -> "Invalid email address entered."
    throwE err

retryOnce = userLogin `catchE` wrongPasswordHandler
retryForever = userLoginRetryOnWrongPassword

loginDialogue :: EitherIO LoginError ()
loginDialogue = do
    token <- retryOnce `catchE` printError -- :: EitherIO LoginError Text
    liftIO $ T.putStrLn (append "Logged in with token:" token)


-- core problem is that the IO monad is designed for dealing with IO 
-- actions, and it is terrible at handling errors. On the other hand, 
-- the Either monad is great at handling errors, but it can't do IO. 
-- A monad designed to both handle errors and IO actions?!
data EitherIO e a = EitherIO { 
    runEitherIO :: IO (Either e a)
}

instance Functor (EitherIO e) where
    -- f :: (a -> a1) -> EitherIO e a -> EitherIO e a1
    -- or f :: (a -> a1) -> IO (Either e a) -> IO (Either e a1)
    fmap f eio = wrapped
        where
            unwrapped = runEitherIO eio -- :: IO (Either e a)
            -- (fmap f) is of type :: (Either e a) -> (Either e a1)
            -- it maps over (Either e) functor
            -- the outter fmap maps over IO functor
            fmapped = fmap (fmap f) unwrapped
            wrapped = EitherIO fmapped

instance Applicative (EitherIO e) where
    -- pure :: a -> EitherIO e a
    pure = EitherIO . return . return
    -- f :: EitherIO e (a -> a1) 
    -- x :: EitherIO e a
    -- f <*> x :: EitherIO e (a -> a1) -> EitherIO e a -> EitherIO e a1
    -- runEitherIO f :: IO (Either e (a -> a1))
    -- runEitherIO x :: IO (Either e a)
    ----------- we want IO (Either e a1)
    -- <*> :: Either e (a -> a1) -> Either e a -> Either e a1
    f <*> x = EitherIO $ (<*>) <$> (runEitherIO f) <*> (runEitherIO x)
    -- or use EitherIO $ liftA2 (<*>) (runEitherIO f) (runEitherIO x)
    -- there is an issue with this implementation. It should not evaluate
    -- the right side even when the left side ends up with a failure.

-- EitherIO String (String -> String)
fbad = liftEither $ Left "no function"
fgood = liftEither $ Right ('0':)
getArg = liftIO getLine

-- due to the implementation of <*>, 
-- runEitherIO $ fgood <*> getArg will ask for string input
-- runEitherIO $ fbad <*> getArg will also ask for string input

instance Monad (EitherIO e) where
    return = pure
    -- x :: EitherIO e a
    -- f :: (a -> EitherIO e a1)
    -- >>= :: EitherIO e a -> (a -> EitherIO e a1) -> EitherIO e a1
    -- unwrapped = runEitherIO x :: IO (Either e a)
    -- g :: Either e a -> IO (Either e a1)
    -- >>= :: IO (Either e a) -> (Either e a -> IO (Either e a1)) -> IO (Either e a1)
    x >>= f = EitherIO $ unwrapped >>= g
        where 
            unwrapped = runEitherIO x
            g ei = case ei of 
                Right a -> runEitherIO $ f a
                Left e -> return $ Left e -- can't just reuse the pattern

