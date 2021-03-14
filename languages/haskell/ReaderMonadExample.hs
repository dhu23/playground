import Control.Monad.Reader

-- this is the abstract syntax representation of a template
data Template = T String -- Text 
              | V Template -- Variable
              | Q Template -- Quote
              | I Template [Definition] -- Include
              | C [Template] -- Compound
              deriving Show

data Definition = D Template Template deriving Show

-- our environment consists of an association list of named templates and
-- an association list of named variable values

data Environment = Env { templates :: [(String, Template)],
                         variables :: [(String, String)] }

lookupVar :: String -> Environment -> Maybe String
lookupVar name env = lookup name (variables env)

lookupTemplate :: String -> Environment -> Maybe Template
lookupTemplate name env = lookup name (templates env)

addDefs :: [(String, String)] -> Environment -> Environment
addDefs defs env = env { variables = defs ++ (variables env) }

resolveDef :: Definition -> Reader Environment (String, String)
resolveDef (D t d) = do 
    name <- resolve t
    value <- resolve d
    return (name, value)

resolve :: Template -> Reader Environment String
resolve (T s) = return s
resolve (V t) = do
    varName <- resolve t
    varValue <- asks (lookupVar varName)
    return $ maybe "" id varValue
resolve (Q t) = do
    tmplName <- resolve t
    body <- asks (lookupTemplate tmplName)
    return $ maybe "" show body
resolve (I t ds) = do
    tmplName <- resolve t
    body <- asks (lookupTemplate tmplName)
    case body of 
        Just t' -> do 
            defs <- mapM resolveDef ds
            local (addDefs defs) (resolve t')
        Nothing -> return ""
resolve (C ts) = (liftM concat) (mapM resolve ts)


main = print 5
