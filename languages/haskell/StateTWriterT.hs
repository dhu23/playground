import Control.Monad.Trans.Class
import Control.Monad.Trans.Writer
import Control.Monad.Trans.State

-- example of code generator

indent :: Monad m => StateT Int m ()
indent = do
    i <- get
    put $ (i+1)

undent :: Monad m => StateT Int m ()
undent = do
    i <- get
    if i > 0
       then put $ (i-1)
       else put 0


writeLine :: String -> WriterT String (StateT Int IO) ()
writeLine s = do
    i <- lift get
    tell (replicate (4*i) ' ') >> tell s >> tell "\n"


exampleCode :: WriterT String (StateT Int IO) ()
exampleCode = do
    writeLine "struct Position"
    writeLine "{" >> lift indent
    writeLine "int x;"
    writeLine "int y;" >> lift undent
    writeLine "};"


runCodeGen = evalStateT (execWriterT exampleCode) 0 >>= putStrLn


writeLine2 :: String -> StateT Int (WriterT String IO) ()
writeLine2 s = do
    i <- get
    lift $ tell (replicate (4*i) ' ') 
    lift $ tell s 
    lift $ tell "\n"


exampleCode2 :: StateT Int (WriterT String IO) ()
exampleCode2 = do
    writeLine2 "struct Position"
    writeLine2 "{" >> indent
    writeLine2 "int x;"
    writeLine2 "int y;" >> undent
    writeLine2 "};"

runCodeGen2 = execWriterT (evalStateT exampleCode2 0) >>= putStrLn

    
