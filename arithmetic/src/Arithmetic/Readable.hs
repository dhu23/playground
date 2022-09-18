module Arithmetic.Readable
  ( Readable
  , fromStr
  , toStr
  ) where


class Readable r where
  fromStr :: String -> Maybe r
  toStr :: r -> String
