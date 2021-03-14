module Main where

import Lib
import qualified Geometry
import qualified FixedIncome

main :: IO ()
--main = someFunc
main = putStrLn $ show $ map Geometry.cubeVolume [1, 2, 3, 4]
