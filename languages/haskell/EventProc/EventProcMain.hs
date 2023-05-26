module Main where

import Control.Monad (forever)
import Control.Monad.IO.Class (liftIO)
import Control.Monad.Trans.State.Strict (evalStateT, modify, StateT, get)
--import Control.Monad.State.Strict (StateT, evalStateT, get, put)
--import Data.Sequence (Seq, empty, viewl, (<|))

--type Event = Int

--processEvent :: Event -> StateT (Seq Event) IO ()
--processEvent event = do
--  eventQueue <- get
--  let updatedQueue = eventQueue <| event
--  put updatedQueue
--  liftIO $ putStrLn $ "Processing event: " ++ show event
--
--processEvents :: [Event] -> StateT (Seq Event) IO ()
--processEvents events = sequence_ $ map processEvent events

--main :: IO ()
--main = do
--  let eventStream = [1..]
--  evalStateT (forever $ do
--    event <- liftIO getLine 
--    processEvents [read event]
--    ) empty


data Event = EventA | EventB | EventC deriving Show

processEvent :: Event -> StateT [Event] IO ()
processEvent event = do
  modify (++ [event])
  liftIO $ putStrLn $ "adding to queue: " ++ show event
  evts <- get
  liftIO $ print evts

processEvents :: [Event] -> IO ()
processEvents events = do
  let initialState = []
  _ <- evalStateT (sequence $ map processEvent events) initialState
  return ()

main :: IO ()
main = forever $ do
  eventStr <- getLine
  case eventStr of 
    "a" -> processEvents [EventA]
    "b" -> processEvents [EventB]
    "c" -> processEvents [EventC]
    _ -> putStrLn "Invalid event"

