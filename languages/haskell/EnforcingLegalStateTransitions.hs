{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE GADTs #-}
{-# LANGUAGE GeneralizedNewtypeDeriving #-}
{-# LANGUAGE TypeFamilies #-}
module EnforcingLegalStateTransitions where

import Control.Monad.IO.Class
import Data.List.NonEmpty
import Data.Semigroup
import qualified Data.Text.IO as T

-- phantom types
data NoItems 
data HasItems
data NoCard
data CardSelected
data CardConfirmed
data OrderPlaced

class Checkout m where
  type State m :: * -> *

initial :: m (State m NoItems)
