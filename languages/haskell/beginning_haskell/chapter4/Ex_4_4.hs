
class Priceable p where
  price :: p -> Double

totalPrice :: Priceable p => [p] -> Double
totalPrice = sum . map price

data TimeMachine = TimeMachine 
    { productName :: String
    , productPrice :: Double
    }

data TravelGuide = TravelGuide 
    { guideName :: String
    , guidePrice :: Double
    }

data MaintainTool = MaintainTool 
    { toolName :: String
    , toolPrice :: Double 
    }

instance Priceable TimeMachine where
  price = productPrice

instance Priceable TravelGuide where
  price = guidePrice

instance Priceable MaintainTool where
  price = toolPrice

data TimeStoreItem 
    = TimeMachineItem { itemName :: String
                      , itemPrice :: Double
                      } 
    | TravelGuideItem { itemName :: String
                      , itemPrice :: Double
                      }
    | MaintainToolItem { itemName :: String
                       , itemPrice :: Double
                       }
    deriving Show

instance Priceable TimeStoreItem where
  price TimeMachineItem { itemName=name, itemPrice=px } = px
  price TravelGuideItem { itemName=name, itemPrice=px } = px
  price MaintainToolItem { itemName=name, itemPrice=px } = px

main = do 
    print $ totalPrice [ TimeMachine "tm1" 100.0
                       , TimeMachine "tm2" 200.0
                       ]
    print $ totalPrice [ TimeMachineItem "tmi1" 150.0
                       , TravelGuideItem "tgi1" 210.0
                       , MaintainToolItem "mti1" 120.0
                       ]
