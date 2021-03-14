import Data.List (intercalate)
import Control.Monad (forM_)

smalls = [ ""
         , "One"
         , "Two"
         , "Three"
         , "Four"
         , "Five"
         , "Six"
         , "Seven"
         , "Eight"
         , "Nine" 
         , "Ten"
         , "Eleven"
         , "Twelve"
         , "Thirteen"
         , "Fourteen"
         , "Fifteen"
         , "Sixteen"
         , "Seventeen"
         , "Eighteen"
         , "Nineteen" ]
tens = [ ""
       , ""
       , "Twenty"
       , "Thirty"
       , "Forty"
       , "Fifty"
       , "Sixty"
       , "Seventy"
       , "Eighty"
       , "Ninety" ]

hundred = "Hundred"

units = [ ""
        , "Thousand"
        , "Million"
        , "Billion" ]

toEnglish :: Int -> String
toEnglish n 
    | n < 0 = toString $ ["Negative", toEnglish (-n)]
    | n == 0 = "Zero"
    | n <= 19 = smalls !! n
    | n < 100 = toString $ if n `mod` 10 /= 0 
                           then [tens !! (n `div` 10), toEnglish (n `mod` 10)]
                           else [tens !! (n `div` 10)]
    | n < 1000 = toString $ 
                 if n `mod` 100 /= 0
                 then [ toEnglish (n `div` 100)
                      , hundred
                      , toEnglish (n `mod` 100)]
                 else [toEnglish (n `div` 100), hundred]
    | otherwise = toString $ reverse $ 
                  zipWith concatStr (map toEnglish $ toThreeDigit n) units
    where
        toString = intercalate " "
        toThreeDigit :: Int -> [Int]
        toThreeDigit x
            | x < 1000 = [x]
            | otherwise = let rem = x `mod` 1000
                              x' = x `div` 1000
                          in if x' == 0
                             then [rem] 
                             else rem : (toThreeDigit x')
        concatStr a b
            | null b = a
            | otherwise = toString [a, b]

main = do
    forM_ [1..99] $ \i -> print (i, toEnglish i)
    forM_ [123, 1234, 12345, 123456, 1234567, 12345678, 123456789] $ \i ->
        print (i, toEnglish i)
