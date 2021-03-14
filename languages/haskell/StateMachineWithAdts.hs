{-# LANGUAGE OverloadedStrings #-}
module StateMachineWithAdts where

import Control.Monad (foldM)
import Data.List.NonEmpty
import Data.Text (Text)
import Text.Printf (printf)

data CartItem = CartItem 
    { itemName :: String
    , itemPrice :: Double 
    } deriving (Show)

data Card = Card String deriving Show

data CheckoutState 
    = NoItems
    | HasItems (NonEmpty CartItem)
    | NoCard (NonEmpty CartItem)
    | CardSelected (NonEmpty CartItem) Card
    | CardConfirmed (NonEmpty CartItem) Card
    | OrderPlaced
    deriving (Show)

data CheckoutEvent
    = Select CartItem
    | Checkout
    | SelectCard Card
    | Confirm
    | PlaceOrder 
    | Cancel 
    deriving (Show)

-- checkout :: CheckoutState -> CheckoutEvent -> IO CheckoutState
-- a state transition then returns of IO of the next state. 

-- FSM stands for Finite State Machine
type FSM s e = s -> e -> IO s

checkout :: FSM CheckoutState CheckoutEvent
checkout NoItems (Select item) = 
    return (HasItems (item :| []))

checkout (HasItems items) (Select item) = 
    return (HasItems (item <| items))

checkout (HasItems items) Checkout = 
    return (NoCard items)

checkout (NoCard items) (SelectCard card) = 
    return (CardSelected items card)

checkout (CardSelected items card) Confirm = 
    return (CardConfirmed items card)

checkout state Cancel = 
    case state of 
      NoCard items -> return (HasItems items)
      CardSelected items _ -> return (HasItems items)
      CardConfirmed items _ -> return (HasItems items)
      _ -> return state

checkout (CardConfirmed items card) PlaceOrder = do
    chargeCard card (calculatePrice items) 
    return OrderPlaced

checkout state _ = return state




calculatePrice = sum . fmap itemPrice

chargeCard (Card cardNum) price = 
    putStrLn $ "Charging card:" ++ cardNum ++ " for " ++ show price

runFsm :: Foldable f => FSM s e -> s -> f e -> IO s
runFsm = foldM

withLogging :: (Show s, Show e) => FSM s e -> FSM s e
withLogging fsm s e = do 
    s' <- fsm s e
    printf "- %s x %s -> %s\n" (show s) (show e) (show s')
    return s'


testFSM1 = runFsm 
    (withLogging checkout)
    NoItems
    [ Select (CartItem "potatoes" 23.95)
    , Select (CartItem "fish" 168.50)
    , Checkout
    , SelectCard (Card "0000-0000-0000-0000")
    , Confirm
    , PlaceOrder
    ]


testFSM2 = runFsm
    (withLogging checkout)
    NoItems
    [ Select (CartItem "potatoes" 23.95)
    , Select (CartItem "fish" 168.50)
    , Checkout
    , Cancel
    ]

