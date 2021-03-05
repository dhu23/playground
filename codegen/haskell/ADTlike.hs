-- generation of algebraic data type like classes in C++
import Control.Monad.Trans.Class (lift)
import Control.Monad.Trans.State (StateT, get, put, runStateT, evalStateT)

import Control.Monad.Trans.Writer (Writer, tell, execWriter)
import Control.Monad (mapM_)

import Data.Char (toLower)
import Data.List (intercalate)
import Data.Set (fromList, toList)

type IndentLevel = Int
type CodeLine = (IndentLevel, String)


indent :: Monad m => StateT Int m ()
indent = do
    i <- get
    put (i+1)

undent :: Monad m => StateT Int m ()
undent = do
    i <- get
    if i > 0
       then put (i-1)
       else put 0


writeLine :: String -> StateT Int (Writer [CodeLine]) ()
writeLine s = do
    i <- get
    lift $ tell [(i, s)]

toString :: Int -> Int -> StateT Int (Writer [CodeLine]) () -> String
toString initIndent indentSize codegen = 
    intercalate "\n" $ map (toCodeString indentSize) lines
      where 
        lines = execWriter $ evalStateT codegen initIndent 
        toCodeString indentSize (n, s) = replicate (n*indentSize) ' ' ++ s


block :: Bool -> Bool -> StateT Int (Writer [CodeLine]) () -> StateT Int (Writer [CodeLine]) ()
block hasSemicolon useIndent codegen = do 
    writeLine "{"
    if useIndent
       then indent >> codegen >> undent
       else codegen
    if hasSemicolon
       then writeLine "};"
       else writeLine "}"


type TypeName = String
type FieldName = String
type EnumName = String
type SourceFile = String

camelCase [] = []
camelCase (x:xs) = (toLower x):xs

private s = s ++ "_"

data DataType
    = Enumerate TypeName [EnumName] -- a list of enum values
    | BuiltIn TypeName 
    | Sequence TypeName [(FieldName, DataType)] -- mimiking sum type
    | Choice TypeName [DataType] -- mimiking product type
    | Nullable DataType
    | Array DataType
    deriving Show


class CppType a where
  tname :: a -> String -- name of the type
  vname :: a -> String -- name as variable in functions (capitalized)
  mname :: a -> String -- name as data member in definition

  headerIncludes :: a -> [String]
  cppIncludes :: a -> [String]
  genDecl :: a -> StateT Int (Writer [CodeLine]) ()
  genDefi :: a -> StateT Int (Writer [CodeLine]) ()

instance CppType DataType where
  tname (Enumerate tn _) = tn
  tname (BuiltIn tn) = tn
  tname (Sequence tn _) = tn
  tname (Choice tn _) = tn
  tname (Nullable t) = "std::optional<" ++ tname t ++ ">"
  tname (Array t) = "std::vector<" ++ tname t ++ ">"

  vname (Nullable t) = vname t
  vname (Array t) = vname t
  vname t = tname t

  mname t = private $ camelCase $ vname t

  headerIncludes (BuiltIn tn)
    | tn == "std::string" = ["<string>", "<iostream>"]
    | otherwise = []
  headerIncludes (Enumerate _ _) = ["<string>", "<iostream>"]
  headerIncludes (Nullable _) = ["<optional>", "<iostream>"]
  headerIncludes (Array _) = ["<vector>", "<iostream>"]
  headerIncludes _ = return "<iostream>"

  cppIncludes (Enumerate _ _) = ["<unordered_map>"]
  cppIncludes _ = []

  genDecl (Enumerate typeName enumVals) = enumerateDecl typeName enumVals
  genDecl (BuiltIn _) = return ()
  genDecl (Sequence typeName members) = sequenceDecl typeName members
  genDecl (Choice typeName members) = choiceDecl typeName members
  genDecl (Nullable t) = return ()
  genDecl (Array t) = return ()

  genDefi (Enumerate typeName enumVals) = enumerateDefi typeName enumVals
  genDefi (BuiltIn _) = return ()
  genDefi (Sequence typeName members) = return ()
  genDefi (Choice typeName members) = choiceDefi typeName members
  genDefi (Nullable t) = return ()
  genDefi (Array t) = return ()


enumClassDecl :: TypeName -> [EnumName] -> StateT Int (Writer [CodeLine]) ()
enumClassDecl typeName enumVals = do
    writeLine "enum class Value : int"
    block True True $ writeEnumLines $ zip [0..] enumVals
      where
        enumCount = length enumVals
        writeEnumLines [] = return ()
        writeEnumLines [(idx,ev)] = writeLine $ ev ++ " = " ++ show idx
        writeEnumLines ((idx,ev):more) = do
            writeLine $ ev ++ " = " ++ show idx ++ ","
            writeEnumLines more


enumClassTypeName typename = typename ++ "::Value"


enquote s = "\"" ++ s ++ "\""

enumerateDecl :: TypeName -> [EnumName] -> StateT Int (Writer [CodeLine]) ()
enumerateDecl typeName enumVals = do
    writeLine $ "struct " ++ typeName
    block True True $ do
        enumClassDecl typeName enumVals
        mapM_ writeLine $
            [ "static bool fromInt(int in," ++ vtype ++ "& out);"
            , "static int toInt(" ++ vtype ++ " in);"
            , "static bool fromString(const std::string& in, " ++ vtype ++ "& out);"
            , "static std::string toString(" ++ vtype ++ " in);"
            , "static int size();"
            ]
      where 
        vtype = enumClassTypeName typeName


enumerateDefi :: TypeName -> [EnumName] -> StateT Int (Writer [CodeLine]) ()
enumerateDefi typeName enumVals = do
    enumerateFromIntDefi typeName enumVals
    enumerateToIntDefi typeName enumVals 
    enumerateFromStringDefi typeName enumVals
    enumerateToStringDefi typeName enumVals
    enumerateSizeDefi typeName enumVals


enumerateFromIntDefi typeName enumVals = do
    writeLine $ "bool " ++ typeName ++ "::fromInt(int in, " ++ vtype ++ "& out)"
    block False True writeSwitchLines
      where
        vtype = enumClassTypeName typeName
        writeCaseLine (idx, ev) = writeLine $ 
            "case " ++ show idx ++ ": out = Value::" ++ ev ++ "; return true;"
        writeSwitchLines = do
            writeLine "switch(in)"
            block False False $ do
                mapM_ writeCaseLine $ zip [0..] enumVals
                writeLine "default: return false;"


enumerateToIntDefi typeName enumVals = do
    writeLine $ "int " ++ typeName ++ "::toInt(" ++ vtype ++ " in)"
    block False True writeSwitchLines
      where 
        vtype = enumClassTypeName typeName
        writeCaseLine (idx, ev) = writeLine $
            "case Value::" ++ ev ++ ": return " ++ show idx ++ ";"
        writeSwitchLines = do
            writeLine "switch(in)"
            block False False $ do 
                mapM_ writeCaseLine $ zip [0..] enumVals
                writeLine "default: return -1;"


enumerateFromStringDefi typeName enumVals = do
    writeLine $ "bool " ++ typeName ++ "::fromString(const std::string& in, " ++ vtype ++ "& out)"
    block False True $ (writeUnorderedMap >> writeMapLookup)
      where
        vtype = enumClassTypeName typeName

        writeUnorderedMap = do
            writeLine "static const std::unordered_map<std::string, Value> m"
            block True True $ writeMapLines enumVals

        writeMapLines [] = return ()
        writeMapLines [ev] = writeLine $ "{" ++ (enquote ev) ++ ", Value::" ++ ev ++ "}"
        writeMapLines (ev:more) = do
            writeLine $ "{" ++ (enquote ev) ++ ", Value::" ++ ev ++ "},"
            writeMapLines more

        writeMapLookup = do
            writeLine "auto it = m.find(in);"
            writeLine "if (it == m.end())"
            block False True $ writeLine "return false;"
            writeLine "out = it->second;"
            writeLine "return true;"


enumerateToStringDefi typeName enumVals = do
    writeLine $ "std::string " ++ typeName ++ "::toString(" ++ vtype ++ " in)"
    block False True writeSwitchLines
      where 
        vtype = enumClassTypeName typeName
        writeCaseLine ev = writeLine $ 
            "case Value::" ++ ev ++ ": return " ++ enquote ev ++ ";"

        writeSwitchLines = do
            writeLine "switch(in)"
            block False False $ do
                mapM_ writeCaseLine enumVals
                writeLine "default: return \"\";"


enumerateSizeDefi typeName enumVals = do
    writeLine $ "int " ++ typeName ++ "::size()"
    block False True $ writeLine $ "return " ++ (show (length enumVals)) ++ ";"


-- no Defi is needed for Sequence type
sequenceDecl :: TypeName -> [(String, DataType)] -> StateT Int (Writer [CodeLine]) ()
sequenceDecl typeName members = do
    writeLine $ "class " ++ typeName
    block True True $ do
        mapM_ writeDataLine members
        undent >> writeLine "public:" >> indent
        writeInterface
    
      where
        writeDataLine (n, t) = writeLine $ tname t ++ " " ++ private n ++ ";"
        writeInterface = do
            writeLine $ typeName ++ "() {}"
            mapM_ writeGetterLines members
            writeLine $ "std::ostream& print(std::ostream& os) const;"
        writeGetterLines (n, t) = do
            writeLine $ tname t ++ "& " ++ n ++ "() { return " ++ private n ++ "; }"
            writeLine $ "const " ++ tname t ++ "& " ++ n ++ "() const { return " ++ private n ++ "; }"


choiceDecl :: TypeName -> [DataType] -> StateT Int (Writer [CodeLine]) ()
choiceDecl _ [] = return ()
choiceDecl typeName members = do
    writeLine $ "class " ++ typeName
    block True True $ do
        writeDataLine
        undent >> writeLine "public:" >> indent
        writeInterface
      where 
        writeDataLine = do 
            writeUnionDef
            writeLine "U data_;" 
            writeLine "int type_;"
        writeUnionLine m = writeLine $ tname m ++ " " ++ mname m ++ ";"
        writeUnionDef = do
            writeLine "union U"
            block True True $ do
                mapM_ writeUnionLine members
                let t0 = head members
                writeLine $ "U() { new(&" ++ mname t0 ++ ") " ++ tname t0 ++ "(); }"
        writeInterface = do
            writeLine $ typeName ++ "();"
            writeLine "void reset() { type_ = -1; }"
            mapM_ writeIsTypeLine members
            mapM_ writeMakeTypeLine members
            mapM_ writeGetterLines members
            writeLine $ "std::ostream& print(std::ostream& os) const;"
        writeIsTypeLine t = writeLine $ "bool is" ++ tname t ++ "() const;"
        writeMakeTypeLine t = writeLine $ tname t ++ "& make" ++ tname t ++ "();"
        writeGetterLines t = do
            writeLine $ tname t ++ "& " ++ camelCase (vname t) ++ "() { return data_." ++ mname t ++ "; }"
            writeLine $ "const " ++ tname t ++ "& " ++ camelCase (vname t) ++ "() const { return data_." ++ mname t ++ "; }"


choiceDefi :: TypeName -> [DataType] -> StateT Int (Writer [CodeLine]) ()
choiceDefi _ [] = return ()
choiceDefi typeName members = do
    writeCtor
    mapM_ writeIsTypeDefi $ zip [0..] members
    mapM_ writeMakeTypeDefi $ zip [0..] members
      where
        writeCtor = do
            writeLine $ typeName ++ "::" ++ typeName ++ "():"
            indent >> writeLine "data_()," >> writeLine "type_(-1)" >> undent
            writeLine "{" >> writeLine "}"
            
        writeIsTypeDefi (idx, m) = do
            let tn = tname m
            writeLine $ "bool " ++ typeName ++ "::is" ++ tn ++ "() const"
            block False True $ writeLine ("return type_ == " ++ show idx ++ ";")

        writeMakeTypeDefi (idx, m) = do
            let tn = tname m
            writeLine $ tn ++ "& " ++ typeName ++ "::make" ++ tn ++ "()"
            block False True $ do
                writeLine $ "new (&data_." ++ mname m ++ ") " ++ tn ++ ";"
                writeLine $ "type_ = " ++ show idx ++ ";"
                writeLine $ "return data_." ++ mname m ++ ";"


-- Test data types 
shiftType = Enumerate "Shift" ["Parallel", "Wave", "Tent"]

intType = BuiltIn "int"

doubleType = BuiltIn "double"

boolType = BuiltIn "bool"

stringType = BuiltIn "std::string"

parallelMethodType = Sequence "ParallelMethod" []

waveMethodType 
    = Sequence "WaveMethod" $
    [ ("left", intType)
    , ("right", intType)
    , ("upSlope", boolType)
    ]

tentMethodType 
    = Sequence "TentMethod" $
    [ ("left", intType)
    , ("mid", intType)
    , ("right", intType)
    ]

shiftMethodType
    = Choice "ShiftMethod" $
    [ parallelMethodType
    , waveMethodType
    , tentMethodType
    ]


include file = do
    writeLine $ "#include " ++ file


mtgeTypes = 
    [ shiftType
    , parallelMethodType
    , waveMethodType
    , tentMethodType
    , shiftMethodType
    ]

mtgeObjName = "mtge_shift"

codeGen types objectName = do
    let toHeaderFile = writeFile (objectName ++ ".h") . toString 0 4 
        toCppFile = writeFile (objectName ++ ".cpp") . toString 0 4
        allIncludes = toList . fromList . concat 

    toHeaderFile $ do 
        mapM_ include $ allIncludes $ map headerIncludes types
        mapM_ genDecl types

    toCppFile $ do
        include $ "\"" ++ objectName ++ ".h\""
        mapM_ include $ allIncludes $ map cppIncludes types
        mapM_ genDefi types

------------------- test case 2
byPhoneType = Sequence "ByPhone" [ ("number", stringType) ]

byMailType 
  = Sequence "ByMail" $ 
      [ ("streetNum", intType)
      , ("streetName", stringType)
      , ("city", stringType)
      , ("state", stringType)
      ]

byEmailType 
  = Sequence "ByEmail" $
      [ ("useName", stringType)
      , ("domain", stringType)
      ]

contactType = Choice "Contact" $ 
    [ Nullable byPhoneType
    , Nullable byMailType
    , Nullable byEmailType 
    ]

contactBookType = Sequence "ContactBook" [ ("book", Array contactType) ]


contactTypes = 
    [ byPhoneType
    , byMailType
    , byEmailType
    , contactType
    , contactBookType
    , Nullable byPhoneType
    , Array contactType
    ]

contactObjName = "contact_book"


