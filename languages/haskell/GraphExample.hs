import qualified Data.Graph as G

-- connection
-- Vermont -> VT
-- New Hampshire -> NH
-- Massachusetts -> MA
-- Connecticut -> CT
-- Rhode Island -> RI
-- Maine -> ME
--
-- VT -> NH, MA
-- CT -> MA, RI
-- RI -> MA, CT
-- MA -> CT, RI, NH, VT
-- NH -> VT, MA, ME
-- ME -> NH

data NEState = VT | NH | MA | CT | RI | ME 
               deriving (Show, Eq, Ord)

newEngland = G.graphFromEdges
           $ [ ("Vermont", VT, [NH, MA])
             , ("New Hamshire", NH, [VT, MA, ME])
             , ("Massachusetts", MA, [CT, RI, NH, VT])
             , ("Connecticut", CT, [MA, RI])
             , ("Rhode Island", RI, [MA, CT])
             , ("Maine", ME, [NH]) ]


