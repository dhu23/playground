module Arithmetic.Evaluable.Class where

class Evaluable e where
  eval :: e -> e -- evaluate E to something simpler
