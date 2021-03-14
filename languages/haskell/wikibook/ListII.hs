import qualified Data.List as DL

elementWiseNegation = map negate

divisor p = [ f | f <- [1..p], p `mod` f == 0]

divisorsOf = map divisor

-- run length encoding
rleEncode :: String -> [(Int, Char)]
rleEncode = map encode . DL.group
    where encode cs = (length cs, head cs)


rleDecode :: [(Int, Char)] -> String
rleDecode = DL.concat . map decode
    where decode (l, c) = replicate l c
