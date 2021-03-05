import Data.List (intercalate)
import Control.Monad (mapM_, forM_)
import Control.Monad.Trans.Class (lift)
import Control.Monad.Trans.State (StateT, get, put, runStateT, evalStateT)
import Control.Monad.Trans.Writer (Writer, tell, execWriter, pass, listen)
import Control.Monad.Trans.Reader (ReaderT, ask, runReaderT)
import Data.Semigroup (Semigroup, (<>))
import Data.Monoid (Monoid, mempty, mappend)

newtype GeneratorMT a 
  = GeneratorMT 
  { run :: ReaderT Int (StateT Int (Writer String)) a
  }

instance Functor GeneratorMT where
  fmap f gmt = GeneratorMT $ fmap f (run gmt)

instance Applicative GeneratorMT where
  pure a = GeneratorMT $ pure a
  f <*> gmt = GeneratorMT $ (run f) <*> (run gmt) 

instance Monad GeneratorMT where
  return = pure
  ma >>= f = GeneratorMT $ (run ma) >>= (\a -> run (f a))

incrIndentLevel :: Monad m => StateT Int m ()
incrIndentLevel = do 
  i <- get
  put $ i+1

decrIndentLevel :: Monad m => StateT Int m ()
decrIndentLevel = do
  i <- get
  put $ max 0 $ i-1

class Generator g where
  indent :: g ()
  undent :: g ()
  append :: String -> g ()
  align :: g Int
  newLine :: g ()
  writeLine :: String -> g ()

-- so far there is only one instance of Generator, GeneratorMT
-- the class is created for the sake of using typeclass
instance Generator GeneratorMT where
  indent = GeneratorMT $ lift incrIndentLevel
  undent = GeneratorMT $ lift decrIndentLevel
  append = GeneratorMT . lift . lift . tell
  align = GeneratorMT $ do
    iSize <- ask
    iLevel <- lift get
    return $ iSize*iLevel
  newLine = append "\n"
  writeLine s = do 
    i <- align 
    append (replicate i ' ') >> append s

--indentedBlock :: GeneratorMT () -> GeneratorMT ()
indentedBlock gen = do 
  writeLine "{" >> newLine
  indent >> gen >> undent
  writeLine "}" >> newLine 

runGen :: Int -> Int -> GeneratorMT () -> String
runGen iSize iLevel g = 
  execWriter $ runStateT (runReaderT (run g) iSize) iLevel

class CodeGen a where
  generate :: (Monad g, Generator g) => a -> g ()

data Expr
  = CInt Int
  | CDouble Double
  | Var String
  | UnaryOp String Expr
  | PostUnaryOp String Expr -- such as i++
  | BinaryOp String Expr Expr -- turn it into each instances, "+", "-" etc
  | Ternary Expr Expr Expr
  | Func String [Expr]
  | ObjFunc String Expr [Expr]
  | Paren Expr

instance CodeGen Expr where
  generate expr = append $ show expr

instance Show Expr where
  show (CInt i) = show i
  show (CDouble d) = show d
  show (Var v) = v
  show (UnaryOp op x) = op ++ show x
  show (PostUnaryOp op x) =  show x ++ op
  show (BinaryOp op l r) = (show l) ++ op ++ (show r)
  show (Ternary cond l r) = (show cond) ++ " ? " ++ (show l) ++ " : " ++ (show r)
  show (Func func args) = func ++ (paren $ intercalate "," $ map show args)
  show (ObjFunc func obj args) = show obj ++ "." ++ show (Func func args)
  show (Paren e) = paren $ show e

paren :: String -> String
paren s = "(" ++ s ++ ")"

type TypeName = String
type VarName = String
data Condition = Condition Expr Block deriving Show

instance CodeGen Condition where
  generate (Condition e body) = do
    writeLine $ show (Paren e)
    generate body

data SimpleStmt 
  = Declaration TypeName VarName [Expr]
  | Assignment Expr Expr
  | Expression Expr 

instance CodeGen SimpleStmt where
  generate ss = append $ show ss

instance Show SimpleStmt where
  show (Declaration tn var args) = tn ++ " " ++ show (Func var args)
  show (Assignment l r) = show l ++ " = " ++ show r
  show (Expression e) = show e
  
data Label = Label String deriving Show
data CaseLabel = CaseLabel Expr deriving Show

instance CodeGen Label where
  generate (Label l) = undent >> writeLine (l ++ ":") >> indent

instance CodeGen CaseLabel where
  generate (CaseLabel e) = undent >> writeLine ("case " ++ show e ++ ":") >> indent

data Block = Block [Stmt] deriving Show

instance CodeGen Block where
  generate (Block body) = case body of 
    [] -> newLine
    stmts -> indentedBlock $ mapM_ generate stmts

data Stmt
  = Simple SimpleStmt
  | Break
  | Continue
  | Return Expr
  | IfStatement Condition [Condition] Block -- if /[else if]/else
  | Switch Expr [Condition] Block
  | WhileLoop Condition
  | ForLoop [SimpleStmt] Expr [SimpleStmt] Block
  deriving Show

instance CodeGen Stmt where
  generate (Simple ss) = writeLine (show ss ++ ";") >> newLine

  generate Break = writeLine "break;" >> newLine
  
  generate Continue = writeLine "continue;" >> newLine
  
  generate (Return e) = writeLine ("return " ++ show e ++ ";") >> newLine
  
  generate (IfStatement ifcond elseifconds elsestmts) = do
    writeLine "if " >> generate ifcond
    forM_ elseifconds $ \ elseifcond -> do
      writeLine "else if " >> generate elseifcond
    generate elsestmts

  generate (WhileLoop cond) = writeLine "while " >> generate cond
  
  generate (Switch e cases defaultblock) = do
    writeLine "switch " >> generate (Paren e)
    indentedBlock $ do
      forM_ cases $ \ (Condition switchcase switchbody) -> do
        generate (CaseLabel switchcase) 
        generate switchbody
      generate (Label "default") >> generate defaultblock
  
  generate (ForLoop inits check steps body) = do
    writeLine "for ("
    generate' inits >> writeLine ";"
    generate check >> writeLine ";"
    generate' steps >> writeLine ")"
    generate body
    where 
      generate' [] = return ()
      generate' (x:[]) = generate x
      generate' (x:xs) = generate x >> writeLine "," >> generate' xs

display :: CodeGen g => g -> IO ()
display = putStrLn . runGen 2 0 . generate


------------------------------- TEST CASES --------------------------------

---- C++ expression example
---- 1
---- 2
---- x
---- x+1
---- x == 5 ? 3 : y.foo(1, 2)
---- ++i // unary operator
---- i--

testExpr1 = CInt 1
testExpr2 = CInt 2
testExprX = Var "x"
testExprAdd = BinaryOp "+" testExprX testExpr1
testExprTernary = 
  Ternary (BinaryOp "==" testExprX (CInt 5)) 
    (CInt 3) 
    (ObjFunc "foo" (Var "y") [testExpr1, testExpr2])
testExprUnary = UnaryOp "++" (Var "i")
testExprPostUnary = PostUnaryOp "--" (Var "i")

printTestExprs = mapM_ display $
  [ testExpr1, testExpr2, testExprX, testExprAdd, testExprTernary
  , testExprUnary, testExprPostUnary
  ]
     
---- C++ statement
---- 5;
---- y = x++;
---- if (obj.like(a)) {
----   doSomething(obj);
---- } else if (obj.like(b)) {
----   doSomething(b);
----   int c(10);
----   while (c-- > 0) {
----     doSideEffect();
----   }
---- }
---- switch (x)
---- {
---- case 1:
---- {
----   x+1;
----   break;
---- }
---- case 2:
---- {
----   x+2;
----   break;
---- }
---- default:
---- }

testSimpleStmt1 = Simple (Expression (CInt 5))
testSimpleStmt2 = Simple (Assignment (Var "y") (PostUnaryOp "++" (Var "x")))

printTestSimpleStmts = mapM_ display [ testSimpleStmt1, testSimpleStmt2]

testIfStatement = 
  let 
    ifcond = Condition 
      (ObjFunc "like" (Var "obj") [Var "a"]) 
      (Block [Simple (Expression (Func "doSomething" [Var "obj"]))])
    elseifconds = 
      [ Condition 
          (ObjFunc "like" (Var "obj") [Var "b"])
          (Block $
            [ Simple (Expression (Func "doSomething" [Var "b"]))
            , Simple (Declaration "int" "c" [CInt 10])
            ])
      ]
  in IfStatement ifcond elseifconds (Block [])

testWhileLoop =
  WhileLoop $ Condition 
    (BinaryOp ">" (PostUnaryOp "--" (Var "c")) (CInt 0))
    (Block [ Simple (Expression (Func "doSideEffect" [])) ])

testSwitch = 
  Switch (Var "x")
    [ Condition (CInt 1) (Block [Return (BinaryOp "+" (Var "x") (CInt 1))])
    , Condition (CInt 2) (Block [Return (BinaryOp "+" (Var "x") (CInt 1))])
    ]
    (Block [])


testForLoop =
  ForLoop
    [Declaration "int" "i" [CInt 0]]
    (BinaryOp "<" (Var "i") (CInt 10))
    [Expression (UnaryOp "++" (Var "i"))]
    (Block [Simple (Assignment (Var "s") (BinaryOp "+" (Var "s") (Var "i")))])
