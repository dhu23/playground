import Control.Monad.Trans.Class
import Control.Monad.Trans.Reader
import Control.Monad.Trans.Writer
import qualified Data.Map as DM

import Text.Read (readMaybe)
import Text.Printf (printf)
import Prelude hiding (log)

type Config = DM.Map String String

serverConfig :: Config
serverConfig = DM.fromList [ ("host", "localhost"), ("port", "7654") ]

-- variation with Reader

getHost :: Reader Config (Maybe String)
getHost = do
    config <- ask
    return $ DM.lookup "host" config


getPort :: Reader Config (Maybe Int)
getPort = do
    config <- ask
    return $ DM.lookup "port" config >>= readMaybe


fromReader :: Monad m => Reader r a -> ReaderT r m a
fromReader = reader . runReader -- unwrap from Reader then put back to ReaderT

log :: (Monad m, MonadTrans t, Monoid w) => w -> t (WriterT w m) ()
log = lift . tell 


getConfig :: ReaderT Config (WriterT String IO) ()
getConfig = do
    hostM <- fromReader getHost
    portM <- fromReader getPort
    let host = maybe "-" id hostM
        port = maybe "-" show portM
    log "\nConfig"
    log "\n======"
    log $ printf "\nhost: %s" host
    log $ printf "\nport: %s" port


readWriteConfig :: IO ()
readWriteConfig = execWriterT (runReaderT getConfig serverConfig) >>= putStrLn


-- variation with ReaderT
getHost2 :: Monad m => ReaderT Config m (Maybe String)
getHost2 = do
    config <- ask
    return $ DM.lookup "host" config


getPort2 :: Monad m => ReaderT Config m (Maybe Int)
getPort2 = do
    config <- ask
    return $ DM.lookup "port" config >>= readMaybe


getConfig2 :: ReaderT Config (WriterT String IO) ()
getConfig2 = do
    hostM <- getHost2
    portM <- getPort2
    let host = maybe "-" id hostM
        port = maybe "-" show portM
    log "\nConfig"
    log "\n======"
    log $ printf "\nhost: %s" host
    log $ printf "\nport: %s" port


readWriteConfig2 :: IO ()
readWriteConfig2 = execWriterT (runReaderT getConfig2 serverConfig) >>= putStrLn


