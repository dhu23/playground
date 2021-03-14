{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE FlexibleInstances #-} 

module SupplyClass 
    { MonadSupply (...)
    , S.Supply
    , S.runSupply ) where

import Control.Monad
import qualified Supply as S

-- | m -> s is a functional dependency, called fundep. 
-- it reads as, such that, m uniquely determines s
class (Monad m) => MonadSupply s m | m -> s where
    next :: m (Maybe s)

instance MonadSupply s (S.Supply s) where
    next = S.next

main = do print 5
