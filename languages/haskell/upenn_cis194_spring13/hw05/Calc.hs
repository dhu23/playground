{-# LANGUAGE TypeSynonymInstances #-}
{-# LANGUAGE FlexibleInstances #-}

module Calc where

import ExprT
--the following is defined in ExprT
--data ExprT = Lit Integer | Add ExprT ExprT | Mul ExprT ExprT 
--             deriving (Show, Eq)

import Parser
--imports a function parseExp

import StackVM(StackExp(PushI), StackVal(IVal), Program, stackVM)
import qualified Data.Map as DM

--exercise 1 
--write version 1 of the calculator: an evaluator for ExprT with 
--eval :: ExprT -> Integer

eval :: ExprT -> Integer
eval (Lit n) = n
eval (Add exp1 exp2) = (eval exp1) + (eval exp2)
eval (Mul exp1 exp2) = (eval exp1) * (eval exp2)

--exercise 2
--another module Parser.hs exports parseExp, a parser for arithmetic 
--expressions. If you pass the constructors of ExprT to it as arguments, 
--it will convert Strings representing arithmetic expressions into values
--of type ExprT. For example:
--parseExp Lit Add Mul "(2+3)*4"
--Just (Mul (Add (Lit 2) (Lit 3)) (Lit 4))
--parseExp Lit Add Mul "2+3*"
--Nothing
--
--
evalStr :: String -> Maybe Integer
evalStr s = case parseExp Lit Add Mul s of
                Nothing -> Nothing 
                Just exp -> Just $ eval exp

--exercise 3
--create a type class called Expr with 3 methods called lit, add and mul
--which parallel the constructors of ExprT. Make an instance of Expr for
--the ExprT type, so that
--mul (add (lit 2) (lit 3)) (lit 4) :: ExprT
--    == Mul (Add (Lit 2) (Lit 3)) (Lit 4)

class Expr a where
    lit :: Integer -> a
    add :: a -> a -> a
    mul :: a -> a -> a

instance Expr ExprT where
    lit = Lit
    add = Add
    mul = Mul


--exercise 4 make instances of Expr for each of the following types:
--Integer - works like the original calculator
--Bool - every literal value less than equal to 0 is interpreted as
--       False and all positive integers are interpreted as True. 
--       addtion is logical or, multiplication is logical and
--MinMax - addition is taken to be the max function, while multiplication
--         is the min function
--Mod7 - all values should be in the range 0..6, and all arithmetic is 
--       done modulo 7, for example 5+3 = 1.

instance Expr Integer where
    lit = id
    add = (+)
    mul = (*)

instance Expr Bool where
    lit = (<=0)
    add = (||)
    mul = (&&)

newtype MinMax = MinMax Integer deriving (Eq, Show)
instance Expr MinMax where
    lit = MinMax
    add (MinMax a) (MinMax b) = MinMax $ max a b
    mul (MinMax a) (MinMax b) = MinMax $ min a b

newtype Mod7 = Mod7 Integer deriving (Eq, Show)
instance Expr Mod7 where
    lit = Mod7 . (`mod` 7)
    add (Mod7 a) (Mod7 b) = lit $ a + b
    mul (Mod7 a) (Mod7 b) = lit $ a * b

testExp :: Expr a => Maybe a
testExp = parseExp lit add mul "(3*-4) + 5"
testInteger = testExp :: Maybe Integer  -- returns Just (-7)
testBool = testExp :: Maybe Bool -- returns Just False
testMM = testExp :: Maybe MinMax -- returns Just (MinMax 5)
testSat = testExp :: Maybe Mod7 -- returns Just (Mod7 0)


--exercise 5
--implement a compiler for arithmetic expressions. simply create an 
--instance of the Expr type class for Program, so that arithmetic 
--expressions can be interpreted as compiled programs. 
--for any arithmetic expression 
--exp :: Expr a => a it should be the case that
--stackVM exp == Right [IVal exp]
--
--finally put together the pieces you have to create a function
--compile :: String -> Maybe Program

fromRight :: Integer -> Either a StackVal -> Integer
fromRight _ (Right (IVal n)) = n
fromRight dflt _ = dflt

-- language extension TypeSynonymInstances enables it
instance Expr Program where
    lit n = [PushI n]
    add p1 p2 = lit $ a + b
        where 
            a = fromRight 0 (stackVM p1) 
            b = fromRight 0 (stackVM p2) 
    mul p1 p2 = lit $ a * b
        where 
            a = fromRight 0 (stackVM p1) 
            b = fromRight 0 (stackVM p2) 

compile :: String -> Maybe Program
compile = parseExp lit add mul 

--exercise 6
--create a new type class HasVars a which contains a single method 
--var :: String -> a, thus types which are instances of HasVars have
--some notion of named variables
--start out by creating a new data type VarExprT which is the same as
--ExprT but with an extra constructor for variables. Make VarExprT an 
--instance of both Expr and HasVars. 

data VarExprT = VLit Integer
              | VAdd VarExprT VarExprT
              | VMul VarExprT VarExprT
              | VVar String
              deriving (Show, Eq)

class HasVars a where 
    var :: String -> a

instance Expr VarExprT where
    lit = VLit
    add = VAdd
    mul = VMul

instance HasVars VarExprT where
    var = VVar

-- now you can right add (lit 3) (var "x") :: VarExprT

--we want to be able to interpret expressions containing variables, 
--given a suitable mapping from variables to values. For storing mappings
--from variables to values, use Data.Map module. 

instance HasVars (DM.Map String Integer -> Maybe Integer) where
    --var :: String -> (DM.Map String Integer -> Maybe Integer)
    var = DM.lookup

--says that these same functions can be interpreted as expressions
--by passing along the mapping to subexpressions and combining results
--appropriately
instance Expr (DM.Map String Integer -> Maybe Integer) where
    --lit :: Integer -> (DM.Map String Integer -> Maybe Integer)
    lit n = \_ -> Just n
    --add :: (DM.Map String Integer -> Maybe Integer) 
    --    -> (DM.Map String Integer -> Maybe Integer) 
    --    -> (DM.Map String Integer -> Maybe Integer) 
    add f1 f2 m = do
        n1 <- f1 m
        n2 <- f2 m
        return (n1 + n2)
    mul f1 f2 m = do
        n1 <- f1 m
        n2 <- f2 m 
        return (n1 * n2)

--to be tested
withVars :: [(String, Integer)] 
         -> (DM.Map String Integer -> Maybe Integer)
         -> Maybe Integer
withVars vs exp = exp $ DM.fromList vs
--:t add (lit 3) (var "x") 
--gives add (lit 3) (var "x") :: (HasVars a, Expr a) => a
-- withVars [("x", 6)] $ add (lit 3) (var "x") --> yields Just 9
-- withVars [("x", 6)] $ add (lit 3) (var "y") --> yields Nothing
-- withVars [("x", 6), ("y", 3)] 
--                    $ mul (var "x") (add (var "y") (var "x"))
--gives Just 54
--
--What?! How did it do this?!
