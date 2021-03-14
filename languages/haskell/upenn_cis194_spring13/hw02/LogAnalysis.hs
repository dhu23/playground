{-# OPTIONS_GHC -Wall #-}
module LogAnalysis where

import Log
import qualified Data.Char as DC

validNum :: String -> Bool
validNum = all DC.isDigit

getErrorType :: [String] -> Maybe LogMessage
getErrorType (e:rc:t:rest) | e == "E" && validNum rc && validNum t = 
    Just $ LogMessage (Error (read rc)) (read t) (unwords rest)
getErrorType _ = Nothing

getWarningType :: [String] -> Maybe LogMessage
getWarningType (w:t:rest) | w == "W" && validNum t = 
    Just $ LogMessage Warning (read t) (unwords rest)
getWarningType _ = Nothing

getInfoType :: [String] -> Maybe LogMessage
getInfoType (i:t:rest) | i == "I" && validNum t = 
    Just $ LogMessage Info (read t) (unwords rest)
getInfoType _ = Nothing



--exercise 1: figure out how to parse an individual message. 
parseMessage :: String -> LogMessage
parseMessage cs = case getErrorType ws of 
    Just e -> e
    Nothing -> case getWarningType ws of
        Just w -> w
        Nothing -> case getInfoType ws of
            Just i -> i
            Nothing -> Unknown (unwords ws)
    where 
        ws = words cs


parse :: String -> [LogMessage]
parse = map parseMessage . lines

--exercise 2
-- make a insert function for MessageTree

insert :: LogMessage -> MessageTree -> MessageTree
insert lm@(LogMessage _ _ _) Leaf = Node Leaf lm Leaf
insert lm@(LogMessage _ ts _) (Node lt n@(LogMessage _ nts _) rt)
    | ts == nts = Node lt lm rt
    | ts > nts = Node lt n (insert lm rt)
    | otherwise = Node (insert lm lt) n rt
insert _ mt = mt

--exercise 3
--once we insert a single LogMessage to a tree, we can build a 
--complete tree from a list of messages

build :: [LogMessage] -> MessageTree
build = foldr insert Leaf 

--exercise 4: inorder traversal

inOrder :: MessageTree -> [LogMessage]
inOrder Leaf = []
inOrder (Node lt n rt) = (inOrder lt) ++ (n : (inOrder rt))

--exercise 5:
--now that we can sort logs, we can extract relative information, meaning
--errors with a severity of at least 50

whatWentWrong :: [LogMessage] -> [String]
whatWentWrong = foldr step [] . sortedLogs
    where 
        sortedLogs = inOrder . build
        step (LogMessage (Error e) _ msg) acc | e > 50 = msg : acc
        step _ acc = acc
