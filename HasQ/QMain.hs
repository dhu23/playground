import Q
import qualified Data.Char as DC
import qualified Control.Monad.Except as CME
import qualified Data.Monoid as DM

fromCtoI :: AtomInt -> CME.Except QErr AtomInt
fromCtoI (C c) 
    | c `elem` "0123456789abcdef" = return $ I (DC.digitToInt c) 
fromCtoI _ = CME.throwError $ TypeErr "expect a Char type"


simpleList1 = toIntQ1 [1, 2, 3]
-- cl1:(1 2 3;4 5 6;7 8 9)

complexList1 = toIntQ2 [[1, 2], [3, 4]]
-- cl2:((1;(2 3));((4 5);6))
complexList2 = L $ [toIntQ 1, toQ1 toIntQ [2, 3]]
complexList3 = L $
    [ L $ [toIntQ 1, toQ1 toIntQ [2, 3]]
    , L $ [toQ1 toIntQ [4, 5], toIntQ 6]
    ]
complexList4 = L $
    [S (I 1), toIntQ1 [2, 3], L [S (I 4), toIntQ1 [5, 6]]]

-- simulate simple list sl2:"123"
simpleList2 = L [S $ C '1', S $ C '2', S $ C '3']
simpleDictionary1 = D $ zip 
    [(S (Symbol "A")), (S (Symbol "B")), (S (Symbol "C"))]
    [(S (I 1)), (S (I 2)), (S (I 3))]

simpleDictionary2 = D $ 
    zip (map toSymbolQ ["A", "B", "C"]) (map toIntQ [1, 2, 3])

testLists = do
    putStrLn "============= test Lists ==============="
    print $ toIntQ1 [1, 2, 3]
    print complexList1
    print complexList2
    print complexList3
    print $ fmap toAtomInt $ L [S $ C '1']
    print simpleList2

testDictionaries = do
    putStrLn "============== Dictionaries =============="
    print $ simpleDictionary1
    print $ simpleDictionary2

testFunctor = do
    putStrLn "=============== Functor =================="
    print $ fmap show simpleList1
    print $ fmap add20 simpleList1
    print $ fmap add20 complexList1
    print $ fmap add20 complexList2
    print $ fmap add20 complexList3
    print $ fmap neg complexList3
    print $ fmap add20 simpleDictionary1
    print $ fmap neg simpleDictionary2
    where add20 = qAdd (I 20)
    
testFoldable = do
    putStrLn "=============== Foldable =================="
    print $ foldMap aIntToStr complexList3
    print $ foldMap (DM.Sum . toInt) complexList4
    
testTraversable = do
    putStrLn "=============== Traversable ================"
    print $ sequenceA $ fmap fromCtoI $ L [S $ C '1', S $ C '2']
    print $ sequenceA $ fmap fromCtoI $ L [S $ C 'x', S $ C '1']
    print $ sequenceA $ fmap fromCtoI $ D
        [ ((S $ Symbol "A"), (S $ C '1'))
        , ((S $ Symbol "B"), (S $ C '2'))
        , ((S $ Symbol "C"), (S $ C 'a'))
        ]
    print $ sequenceA $ fmap fromCtoI $ D
        [ ((S $ Symbol "A"), (S $ C 'x'))
        , ((S $ Symbol "B"), (S $ C '2'))
        , ((S $ Symbol "C"), (S $ C 'a'))
        ]

main = do
    testLists
    testDictionaries
    testFunctor
    testFoldable
    testTraversable
