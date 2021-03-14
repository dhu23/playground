module MonadicWay where

data Term 
    = Con Int
    | Add Term Term 
    deriving Show

eval :: Term -> Int
eval (Con a) = a
eval (Add a b) = eval a + eval b

-- to be able to print out result, the output contains a string part
type MOut a = (a, Output)
type Output = String

formatLine :: Term -> Int -> Output
formatLine t a = "eval (" ++ show t ++ ") ==> " ++ show a ++ " - "

eval0 :: Term -> MOut Int
eval0 (Con a) = (a, formatLine (Con a) a)
eval0 (Add t u) = ((a+b), (x++y++formatLine (Add t u) (a+b)))
  where 
    (a, x) = eval0 t
    (b, y) = eval0 u

bindM :: MOut a -> (a -> MOut b) -> MOut b
bindM m f = (b, x ++ y)
  where
    (a, x) = m
    (b, y) = f a

evalM_1 :: Term -> MOut Int
evalM_1 (Con a) = (a, formatLine (Con a) a)
evalM_1 (Add t u) = bindM (evalM_1 t) (\a -> 
    bindM (evalM_1 u) (\b -> 
        ((a+b), formatLine (Add t u) (a+b))))

evalM_2 :: Term -> MOut Int
evalM_2 (Con a) = (a, formatLine (Con a) a)
evalM_2 (Add t u) = evalM_2 t `bindM` \a ->
    evalM_2 u `bindM` \b ->
        ((a+b), (formatLine (Add t u) (a+b)))

mkM :: a -> MOut a
mkM a = (a, "")

outPut :: Output -> MOut ()
outPut x = ((), x)

evalM_3 :: Term -> MOut Int
evalM_3 (Con a) = outPut (formatLine (Con a) a) `bindM` \_ -> mkM a
evalM_3 (Add t u) = evalM_3 t `bindM` \a ->
    evalM_3 u `bindM` \b ->
        outPut (formatLine (Add t u) (a+b)) `bindM` \_ -> mkM (a+b)

combineM :: MOut a -> MOut b -> MOut b
combineM m f = m `bindM` \_ -> f

evalM :: Term -> MOut Int
evalM (Con a) = outPut (formatLine (Con a) a) `combineM` mkM a
evalM (Add t u) = evalM t `bindM` \a -> 
    evalM u `bindM` \b -> 
        outPut (formatLine (Add t u) (a+b)) `combineM` mkM (a+b)

type MO a = (a, Out)
type Out = String

mkMO :: a -> MO a 
mkMO a = (a, "")

bindMO :: MO a -> (a -> MO b) -> MO b
bindMO m f = (b, x++y) 
  where (a, x) = m
        (b, y) = f a

combineMO :: MO a -> MO b -> MO b
combineMO m f = m `bindMO` \_ -> f

outMO :: Out -> MO ()
outMO x = ((), x)

evalMO :: Term -> MO Int
evalMO (Con a) = outMO (formatLine (Con a) a) `combineMO` mkMO a
evalMO (Add t u) = evalMO t `bindMO` \a -> 
    evalMO u `bindMO` \b ->
        outMO (formatLine (Add t u) (a+b)) `combineMO` mkMO (a+b)
    
-- monadic evaluator with output in do-notation
newtype Eval_IO a = Eval_IO (a, O) deriving (Show)
type O = String

getInt monad doSomething = Eval_IO (newInt, oldOutput ++ newOutput)
  where Eval_IO (oldInt, oldOutput) = monad
        Eval_IO (newInt, newOutput) = (doSomething oldInt)

createEval_IO :: a -> Eval_IO a
createEval_IO int = Eval_IO (int, "")

print_IO :: O -> Eval_IO ()
print_IO string = Eval_IO ((), string)

evalM_4 :: Term -> Eval_IO Int
evalM_4 (Con a) = createEval_IO a
evalM_4 (Add t u) = evalM_4 t `getInt` \a ->
    evalM_4 u `getInt` \b ->
        print_IO (formatLine (Add t u) (a+b)) `getInt` \_ ->
            createEval_IO (a+b)

instance Functor Eval_IO where
  fmap f (Eval_IO (a, s)) = Eval_IO (f a, s)

instance Applicative Eval_IO where
  pure a = createEval_IO a
  (Eval_IO (f, s1)) <*> (Eval_IO (a, s2)) = Eval_IO (f a, s1 ++ s2)

instance Monad Eval_IO where
  return a = createEval_IO a
  (>>=) m f = getInt m f

eval_IO :: Term -> Eval_IO Int
eval_IO (Con a) = do 
    print_IO (formatLine (Con a) a)
    return a
eval_IO (Add t u) = do 
    a <- eval_IO t
    b <- eval_IO u
    print_IO (formatLine (Add t u) (a+b))
    return (a+b)


---------------- Errare monadicum est ----------------------

data M a = Raise Exception
         | Return a
         deriving (Show)
type Exception = String

evalE :: Term -> M Int
evalE (Con a) = Return a

evalE (Add a b) =
    case evalE a of 
      Raise e -> Raise e
      Return a -> 
          case evalE b of 
            Raise e -> Raise e
            Return b ->
                if (a+b) == 42
                   then Raise "The ultimate answer has been computed!I'm tired"
                   else Return (a+b)

-- basic evaluator, monadic, with exceptions
data M1 a = Except Exception
          | Ok { showM :: a }
          deriving (Show)

instance Functor M1 where
  fmap f m = case m of 
               Except e -> Except e
               Ok a -> Ok $ f a

instance Applicative M1 where
  pure a = Ok a
  f <*> m = case f of
              Except fe -> Except fe
              Ok fa -> case m of 
                         Except me -> Except me
                         Ok ma -> Ok (fa ma)

instance Monad M1 where
  return a = Ok a
  m >>= f = case m of
              Except e -> Except e
              Ok a -> f a

raise :: Exception -> M1 a 
raise e = Except e

eval_ME :: Term -> M1 Int
eval_ME (Con a) = do return a
eval_ME (Add t u) = do 
    a <- eval_ME t
    b <- eval_ME u
    if (a+b) == 12
       then raise "The ultimate answer has been computed!I'm tired!"
       else return (a+b)

-- monadic evaluator with output and exceptions
data M2 a = Ex Exception
          | Done { unpack :: (a, O) }
          deriving (Show)

instance Functor M2 where
  fmap f m = case m of
               Ex e -> Ex e
               Done (a, o) -> Done (f a, o)

instance Applicative M2 where
  pure a = Done (a, "")
  f <*> m = case f of
              Ex fe -> Ex fe
              Done (fa, o1) -> case m of 
                                 Ex me -> Ex me
                                 Done (ma, o2) -> Done (fa ma, o1++o2)

-- now we have to concatenate the output AND check for exceptions
instance Monad M2 where
  return a = Done (a, "")
  m >>= f = case m of 
              Ex e -> Ex e
              Done (a, x) -> case (f a) of 
                               Ex e1 -> Ex e1
                               Done (b, y) -> Done (b, x++y)

raise_IOE :: Exception -> M2 a
raise_IOE e = Ex e

print_IOE :: O -> M2 ()
print_IOE x = Done ((), x)

eval_IOE :: Term -> M2 Int
eval_IOE (Con a) = do
    print_IOE (formatLine (Con a) a)
    return a
eval_IOE (Add t u) = do
    a <- eval_IOE t
    b <- eval_IOE u
    let out = formatLine (Add t u) (a+b)
    print_IOE out
    if (a+b) == 42
       then raise_IOE $ out ++ "The ultimate answer was been computed!!I'm tired"
       else return (a+b)

-- basic evaluator, non monadic with a counter
type St a = State -> (a, State)
type State = Int
evalNMS :: Term -> St Int -- think as Term -> State -> (a, State)
evalNMS (Con a) x = (a, x+1)
evalNMS (Add t u) x = let (a, y) = evalNMS t x in
                          let (b, z) = evalNMS u y in 
                              (a+b, z+1)
-- the operation is very similar to the output producing evaluator

-- the evaluator, monadic with a counter
newtype MS a = MS { unpackMSandRun :: (State -> (a, State)) }
mkMS :: a -> MS a
mkMS int = MS (\x -> (int, x))

bindMS :: MS a -> (a -> MS b) -> MS b
bindMS monad doNext  = MS $
    \initialState -> 
        let (oldInt, oldState) = unpackMSandRun monad initialState in
            let (newInt, newState) = unpackMSandRun (doNext oldInt) oldState in
                (newInt, newState)

instance Functor MS where
  fmap f (MS st) = MS $ \init -> let (a, end) = st init in
                                     (f a, end)

instance Applicative MS where
  pure a = mkMS a
  f <*> m = MS $ \s -> 
      let (fa, s1) = unpackMSandRun f s in
          let (ma, s2) = unpackMSandRun m s1 in
              (fa ma, s2)

instance Monad MS where
  return a = mkMS a
  m >>= f = bindMS m f

incState :: MS ()
incState = MS (\s -> ((), s+1))

evalMS :: Term -> MS Int
evalMS (Con a) = do
    incState
    mkMS a
evalMS (Add t u) = do
    a <- evalMS t
    b <- evalMS u
    incState
    return (a+b)

-- the monadic evaluator with output and counter in do-notation
newtype Eval_SIO a = Eval_SIO { unPackMSIOandRun :: State -> (a, State, Output) }

bindMSIO monad doNext = Eval_SIO $
    \initialState -> 
        let (oldInt, oldState, oldOutput) = unPackMSIOandRun monad initialState in
            let (newInt, newState, newOutput) = unPackMSIOandRun (doNext oldInt) oldState in 
                (newInt, newState, oldOutput++newOutput)

mkMSIO int = Eval_SIO $ \s -> (int, s, "")

instance Functor Eval_SIO where
  fmap f m = Eval_SIO $ \s -> 
      let (a, s1, o) = unPackMSIOandRun m s in (f a, s1, o)

instance Applicative Eval_SIO where
  pure a = mkMSIO a
  f <*> m = Eval_SIO $ \s ->
      let (fa, s1, o1) = unPackMSIOandRun f s in
          let (ma, s2, o2) = unPackMSIOandRun m s1 in
              (fa ma, s2, o1++o2)

instance Monad Eval_SIO where
  return a = mkMSIO a
  m >>= f = m `bindMSIO` f

incSIOstate :: Eval_SIO ()
incSIOstate = Eval_SIO $ \s -> ((), s+1, "")

print_SIO :: Output -> Eval_SIO ()
print_SIO x = Eval_SIO $ \s -> ((), s, x)

eval_SIO :: Term -> Eval_SIO Int
eval_SIO (Con a) = do
    incSIOstate
    print_SIO (formatLine (Con a) a)
    return a
eval_SIO (Add t u) = do
    a <- eval_SIO t
    b <- eval_SIO u
    incSIOstate
    print_SIO (formatLine (Add t u) (a+b))
    return (a+b)

example42 = Add (Con 10) (Add (Add (Con 20) (Con 10)) (Con 2))
example54 = Add (Con 6) (Add (Con 16) (Add (Con 20) (Con 12)))

--- the aforementioned examples are a good introduction to monad trans

data Result a = Good a State Output
              | Bad State Output Exception
              deriving Show

newtype Eval_SIOE a = SIOE { runSIOE :: State -> Result a }

instance Functor Eval_SIOE where
  fmap f m = SIOE $ \s ->
      case runSIOE m s of 
        Bad s1 o1 e -> Bad s1 o1 e
        Good a s1 o1 -> Good (f a) s1 o1

instance Applicative Eval_SIOE where
  pure a = SIOE $ \s -> Good a s ""
  f <*> m = SIOE $ \s ->
      case runSIOE f s of
        Bad s1 o1 e1 -> Bad s1 o1 e1
        Good fa s1 o1 -> 
            case runSIOE m s1 of
              Bad s2 o2 e2 -> Bad s2 (o1++o2) e2
              Good ma s2 o2 -> Good (fa ma) s2 (o1++o2)

instance Monad Eval_SIOE where
  return = pure
  m >>= f = SIOE $ \s -> 
      case runSIOE m s of 
        Bad s1 o1 e1 -> Bad s1 o1 e1
        Good ma s1 o1 -> 
            case runSIOE (f ma) s1 of
              Bad s2 o2 e2 -> Bad s2 (o1++o2) e2
              Good ma s2 o2 -> Good ma s2 (o1++o2)

raise_SIOE e = SIOE $ \s -> Bad s "" e

incSIOEstate :: Eval_SIOE ()
incSIOEstate = SIOE $ \s -> Good () (s+1) ""

print_SIOE :: Output -> Eval_SIOE ()
print_SIOE o = SIOE $ \s -> Good () s o

eval_SIOE :: Term -> Eval_SIOE Int
eval_SIOE (Con a) = do 
    incSIOEstate 
    print_SIOE (formatLine (Con a) a)
    return a
eval_SIOE (Add t u) = do
    a <- eval_SIOE t
    b <- eval_SIOE u
    incSIOEstate
    print_SIOE $ formatLine (Add t u) (a+b)
    if (a+b) == 42
       then raise_SIOE "the ultimate answer has been computed!!I'm tired"
       else return (a+b)

runEval exp = 
    case runSIOE (eval_SIOE exp) 0 of
      Bad s o e -> "Error at iteration n." ++ show s ++ 
          " - output stack = " ++ o ++ " - Exception " ++ e
      Good a s o -> "Result = " ++ show a ++ 
          " - iterations = " ++ show s ++ " -output = " ++ o


---- Monadic transformers: StateT monad: A monad container ------------
newtype StateT s m a = StateT { runStateT :: s -> m (a, s) } 

instance Monad m => Functor (StateT s m) where
  fmap f m = StateT $ \s -> do 
      (a, s1) <- runStateT m s
      return (f a, s1)

instance Monad m => Applicative (StateT s m) where
  pure a = StateT $ \s -> return (a, s)
  f <*> m = StateT $ \s -> do
      (fa, s1) <- runStateT f s
      (ma, s2) <- runStateT m s1
      return (fa ma, s2)

instance Monad m => Monad (StateT s m) where
  return = pure
  m >>= f = StateT $ \s -> do
      (ma, s1) <- runStateT m s
      runStateT (f ma) s1

runState :: s -> StateT s m a -> m (a, s)
runState s m = runStateT m s 

evalState :: Functor m => s -> StateT s m a -> m a
evalState s m = fmap fst $ runState s m

execState :: Functor m => s -> StateT s m a -> m s
execState s m = fmap snd $ runState s m

lift :: Monad m => m a -> StateT s m a
lift m = StateT $ \s -> do -- in m a context
    x <- m 
    return (x, s)


-- StateT as a counter, and monadic evaluator with output and exceptions --
data MTa a = FailTa Exception
           | DoneTa { unpackDoneGTa :: (a, O) }
           deriving Show

instance Functor MTa where
  fmap f m = case m of 
               FailTa e -> FailTa e
               DoneTa (a, o) -> DoneTa (f a, o)

instance Applicative MTa where
  pure a = DoneTa (a, "")
  f <*> m = 
      case f of
        FailTa ef -> FailTa ef
        DoneTa (fa, o1) -> 
            case m of
              FailTa em -> FailTa em
              DoneTa (ma, o2) -> DoneTa (fa ma, o1++o2)

instance Monad MTa where
  return = pure
  m >>= f = 
      case m of
        FailTa em -> FailTa em
        DoneTa (am, o1) -> 
            case f am of
              FailTa en -> FailTa en
              DoneTa (an, o2) -> DoneTa (an, o1++o2)

raiseTa_SIOE :: O -> StateT Int MTa a
raiseTa_SIOE = lift . FailTa

printTa_SIOE :: O -> StateT Int MTa ()
printTa_SIOE o = StateT $ \s -> DoneTa (((), s), o)
-- or printTa_SIOE o = lift $ DoneTa ((), o)
-- equivalent
-- printTa_SIOE o = StateT $ \s -> do
--     x <- DoneTa ((), o)
--     return (x, s)

incTaState :: StateT Int MTa ()
incTaState = StateT $ \s -> return ((), s+1)

evalTa_SIOE :: Term -> StateT Int MTa Int
evalTa_SIOE (Con a) = do
    incTaState
    printTa_SIOE (formatLine (Con a) a)
    return a
evalTa_SIOE (Add t u) = do
    a <- evalTa_SIOE t
    b <- evalTa_SIOE u
    incTaState
    printTa_SIOE (formatLine (Add t u) (a+b))
    if (a+b) == 42
       then raiseTa_SIOE "The ultimate answer has been computed!I'm tired"
       else return (a+b)

runEvalTa :: Term -> String
runEvalTa exp = case runStateT (evalTa_SIOE exp) 0 of 
                  FailTa e -> e
                  DoneTa ((r, s), o) -> 
                      "Result = " ++ show r ++ 
                          "; Iteration = " ++ show s ++ 
                              "; Output = " ++ o

runEvalTa1 :: Term -> String
runEvalTa1 exp = case runState 0 (evalTa_SIOE exp) of
                   FailTa e -> e
                   DoneTa ((r, s), o) -> 
                       "Result = " ++ show r ++ 
                           "; Iteration = " ++ show s ++ 
                               "; Output = " ++ o

runEvalTa2 :: Term -> String
runEvalTa2 exp = case evalState 0 (evalTa_SIOE exp) of
                   FailTa e -> e
                   DoneTa (r, o) -> 
                       "Result = " ++ show r ++ 
                           "; Output = " ++ o

runEvalTa3 :: Term -> String
runEvalTa3 exp = case execState 0 (evalTa_SIOE exp) of
                   FailTa e -> e
                   DoneTa (s, o) -> 
                       "Iteration = " ++ show s ++ 
                           "; Output = " ++ o

-- StateT to keep output and counter and monadic evaluator with only exceptions
-- output is taken away from the inner monad and put in the outer one


-- this is basically an Either monad
data MTb a = FailTb Exception
           | DoneTb { unpackDoneTb :: a } -- O is removed
           deriving Show

type StateIO = (O, Int) -- (error, counter) as state

instance Functor MTb where
  fmap f (FailTb e) = FailTb e
  fmap f (DoneTb a) = DoneTb (f a)

instance Applicative MTb where
  pure = DoneTb
  (FailTb e) <*> _ = FailTb e
  (DoneTb _) <*> (FailTb e) = FailTb e
  (DoneTb f) <*> (DoneTb m) = DoneTb (f m)

instance Monad MTb where
  return = pure
  (FailTb e) >>= _ = FailTb e
  (DoneTb m) >>= f = case f m of
                       FailTb e -> FailTb e
                       DoneTb r -> DoneTb r

raiseTb_SIOE :: O -> StateT StateIO MTb a
raiseTb_SIOE = lift . FailTb

printTb_SIOE :: O -> StateT StateIO MTb ()
printTb_SIOE x = StateT $ \(o, s) -> return ((), (o++x, s))

incTbStateIO :: StateT StateIO MTb ()
incTbStateIO = StateT $ \(o, s) -> return ((), (o, s+1))

evalTb_SIOE :: Term -> StateT StateIO MTb Int
evalTb_SIOE (Con a) = do
    incTbStateIO
    printTb_SIOE (formatLine (Con a) a)
    return a
evalTb_SIOE (Add t u) = do
    a <- evalTb_SIOE t
    b <- evalTb_SIOE u
    incTbStateIO 
    printTb_SIOE (formatLine (Add t u) (a+b))
    if (a+b) == 42
       then raiseTb_SIOE $ "The Ultimate Answer has been computed!I'm tired"
       else return (a+b)

runEvalTb :: Term -> String
runEvalTb exp = case runStateT (evalTb_SIOE exp) ("", 0) of 
                  FailTb e -> e
                  DoneTb (r, (o, s)) -> 
                      "Result = " ++ show r ++ 
                          "; Iteration = " ++ show s ++ 
                              "; Output = " ++ o

runEvalTb1 :: Term -> String
runEvalTb1 exp = case runState ("", 0) (evalTb_SIOE exp) of
                   FailTb e -> e
                   DoneTb (r,(o, s)) -> 
                       "Result = " ++ show r ++ 
                           "; Iteration = " ++ show s ++ 
                               "; Output = " ++ o

runEvalTb2 :: Term -> String
runEvalTb2 exp = case evalState ("", 0) (evalTb_SIOE exp) of
                   FailTb e -> e
                   DoneTb r -> "Result = " ++ show r

runEvalTb3 :: Term -> String
runEvalTb3 exp = case execState ("", 0) (evalTb_SIOE exp) of
                   FailTb e -> e
                   DoneTb (o, s) -> 
                       "Iteration = " ++ show s ++ "; Output = " ++ o

-- StateT to keep output, counter and debug. 
-- the monadic evaluator is only for failable computations

data MT a = FailT Exc
          | DoneT { unpackDoneT :: a }
          deriving Show

type Exc = String
type IOStack = [Output]
newtype StateTIO = StateTIO { unPackStateTIO :: (IOStack, Exc, Int) }
    deriving Show

-- MT is the same as MTb
instance Functor MT where
  fmap f (FailT e) = FailT e
  fmap f (DoneT a) = DoneT (f a)

instance Applicative MT where
  pure = DoneT
  (FailT e) <*> _ = FailT e
  (DoneT _) <*> (FailT e) = FailT e
  (DoneT f) <*> (DoneT m) = DoneT (f m)

instance Monad MT where
  return = pure
  (FailT e) >>= _ = FailT e
  (DoneT m) >>= f = case f m of
                       FailT e -> FailT e
                       DoneT r -> DoneT r

stopExecT_SIOE :: Output -> StateT StateTIO MT Int
stopExecT_SIOE exc = StateT $ \s -> FailT exc
-- isn't it the same as StateT $ \s -> do {x <- FailT exc; return (x, s);}

catchT_SIOE exc = StateT $ \(StateTIO (os, e, s)) ->
    return ((), StateTIO (os, "Exception at iteration=" ++ 
        show s ++ ": " ++ exc ++ " - " ++ e, s))

printT_SIOE :: Output -> StateT StateTIO MT ()
printT_SIOE x = StateT $ \(StateTIO (os, e, s)) ->
    return ((), StateTIO (x:os, e, s))

incTstateIO :: StateT StateTIO MT ()
incTstateIO = StateT $ \(StateTIO (os, e, s)) ->
    return ((), StateTIO (os, e, s+1))

evalT_SIOE :: Term -> StateT StateTIO MT Int
evalT_SIOE (Con a) = do
    incTstateIO
    printT_SIOE (formatLine (Con a) a)
    return a
evalT_SIOE (Add t u) = do
    a <- evalT_SIOE t
    b <- evalT_SIOE u
    incTstateIO 
    printT_SIOE (formatLine (Add t u) (a+b))
    case (a+b) of
      42 -> do 
          catchT_SIOE "The ultimate answer has been computed!I'm tired"
          return (a+b)
      11 -> stopExecT_SIOE "11...I do not like this number!"
      otherwise -> return (a+b)

initStateTIO = StateTIO ([], "", 0)

runEvalT :: Term -> String
runEvalT exp = case runStateT (evalT_SIOE exp) initStateTIO of
                 FailT e -> e
                 DoneT (r, StateTIO (o, e, s)) -> "Result = " ++ show r ++
                     ";Iteration = " ++ show s ++ "; Output = " ++ show o ++
                         " - Exceptions = " ++ e




