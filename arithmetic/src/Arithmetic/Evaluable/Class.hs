module Arithmetic.Evaluable.Class where


class Evaluable e where
    eval :: e -> e
