module ReaderMonadEx where

import Control.Monad.Reader (Reader, ask, runReader)
import Data.List (intercalate)
import Prelude hiding (div)

type Html = String
type Email = String


div :: [Html] -> Html
div children = "<div>" ++ combine children ++ "</div>"

h1 :: [Html] -> Html
h1 children = "<h1>" ++ combine children ++ "</h1>"

p :: [Html] -> Html 
p children = "<p>" ++ combine children ++ "</p>"

combine :: [Html] -> Html
combine = intercalate ""

view :: Email -> Html
view email = div [page email]

page :: Email -> Html
page email = div [topNav, content email]

topNav :: Html
topNav = div [h1 ["OurSite.com"]]

content :: Email -> Html
content email = div [ h1 ["Custom Content for " ++ email]
                    , left 
                    , right email
                    ]

left :: Html
left = div [p ["this is the left side"]]

right :: Email -> Html 
right email = div [article email]

article :: Email -> Html
article email = div [p ["this is an article"], widget email]

widget :: Email -> Html
widget email = div [p ["Hey " ++ email ++ ",we've got a great offer for you"]]


-- think Reader Email Html as a wrapped up Email -> Html function
viewR :: Reader Email Html
-- in this case, page :: Reader Email Html
-- page' is of Html type
viewR = pageR >>= \page' -> return $ div [page']

pageR :: Reader Email Html
pageR = contentR >>= \content' -> return $ div [topNav, content']

contentR :: Reader Email Html
contentR = 
    ask >>= \email -> 
        rightR >>= \right' -> return $ 
            div [ h1 ["Custom content for " ++ email]
                , left
                , right'
                ]

rightR :: Reader Email Html
rightR = articleR >>= \article' -> return $ div [article']

articleR :: Reader Email Html
articleR = widgetR >>= \widget' -> return $ 
    div [ p ["this is an article"]
        , widget'
        ]

widgetR :: Reader Email Html
widgetR = 
    ask >>= \email -> 
        return $ div 
            [ p ["Hey " ++ email ++ ",we've got a great offer for you" ]
            ]

