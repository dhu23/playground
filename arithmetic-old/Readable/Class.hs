module Arithmetic.Readable.Class where

class Readable r where
  fromString :: String -> Maybe a
  toString :: a -> String
