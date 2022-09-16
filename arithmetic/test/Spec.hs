import QC.TestDigit (checkDigit)
import QC.TestNatural (checkNatural)
import QC.TestInteger (checkInteger)

main :: IO ()
main = do 
  checkDigit
  checkNatural
  checkInteger
