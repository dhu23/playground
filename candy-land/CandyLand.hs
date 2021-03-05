import qualified Control.Monad.Trans.Reader as R
import qualified Control.Monad.Trans.State as S
import Control.Monad.Trans.Class (lift)
import qualified Data.Map as M
import qualified Data.Array as A
import qualified System.Random as Rand

data Color
  = Red
  | Purple
  | Blue
  | Yellow 
  | Orange
  | Green
  deriving (Show, Eq, Ord)

colors = [Red, Purple, Blue, Yellow, Orange, Green]

data SpecialFood 
  = Cupcake
  | Icecream
  | Gummystar
  | Gingerbreadman
  | Lollipop
  | Popsicle
  | Chocolate
  deriving (Show, Eq, Ord)

foods =
  [ Cupcake, Icecream, Gummystar
  , Gingerbreadman, Lollipop, Popsicle, Chocolate
  ]

data Path
  = PPeppermint
  | PGummybear
  deriving Show

data Tile 
  = TStart
  | TRegular Color 
  | TPassStart Color Path
  | TPassEnd Color Path
  | TLicorice Color
  | TSpecial SpecialFood
  | TRainbow
  deriving Show

tileHasColor :: Color -> Tile -> Bool
tileHasColor _ TStart = False
tileHasColor c1 (TRegular c2) = c1 == c2
tileHasColor c1 (TPassStart c2 _) = c1 == c2
tileHasColor c1 (TPassEnd c2 _) = c1 == c2
tileHasColor c1 (TLicorice c2) = c1 == c2
tileHasColor _ (TSpecial _) = False
tileHasColor _ TRainbow = True

tileIsFood :: SpecialFood -> Tile -> Bool
tileIsFood food (TSpecial fd) = food == fd
tileIsFood food _ = False

data Player
  = PRed
  | PBlue
  | PGreen
  | PYellow
  deriving (Show, Eq, Ord)

data Card 
  = CSingle Color
  | CDouble Color
  | CSpecial SpecialFood
  deriving Show

data Board
  = Board 
  { allTiles :: A.Array Int Tile
  , colorTiles :: M.Map Color [Int]
  , specialTiles :: M.Map SpecialFood Int
  } deriving Show

lastTile :: Board -> Int
lastTile board = snd $ A.bounds $ allTiles board

mkTileArray :: [Tile] -> A.Array Int Tile
mkTileArray ts = A.array (0, len-1) $ zip [0..] ts
  where len = length ts 

mkColorTileMap :: [Tile] -> M.Map Color [Int]
mkColorTileMap ts = M.fromList $ zip colors $ map mkColorPosList colors
  where
    mkColorPosList c = [ i | (i, t) <- zip [0..] ts, tileHasColor c t]

mkSpecialTileMap :: [Tile] -> M.Map SpecialFood Int
mkSpecialTileMap ts = M.fromList foodPosList
  where
    foodPosList = 
      [ (fd, head pos) 
      | fd <- foods
      , let pos = [ i | (i, t) <- zip [0..] ts, tileIsFood fd t]
      , length pos == 1
      ]

mkBoard :: [Tile] -> Board
mkBoard ts = Board arr colorMap foodMap
  where 
    arr = mkTileArray ts
    colorMap = mkColorTileMap ts
    foodMap = mkSpecialTileMap ts

type GameMove = Int -> Int

moveBeyond pos = filter (>pos) 

move :: Monad m => Card -> R.ReaderT Board m GameMove
move (CSingle c) = do 
  board <- R.ask
  return $ \pos ->
    if pos == lastTile board
      then pos
      else 
        case fmap (moveBeyond pos) $ M.lookup c (colorTiles board) of
          Nothing -> lastTile board -- or pos, depending on the moving rule
          Just [] -> lastTile board
          Just (x:_) -> x
move (CDouble c) = do
  movef <- move (CSingle c)
  return $ \pos -> let nextPos = movef pos in movef nextPos
move (CSpecial food) = do
  board <- R.ask
  return $ \pos ->
    if pos == lastTile board
      then pos
      else 
        case M.lookup food (specialTiles board) of
          Nothing -> pos
          Just x -> x

tiles :: [Tile]
tiles = 
  [ TStart
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TPassStart Blue PPeppermint
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TSpecial Cupcake
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TSpecial Icecream
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TPassStart Yellow PGummybear
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TPassEnd Yellow PGummybear
  , TSpecial Gummystar
  , TRegular Blue
  , TRegular Orange
  , TLicorice Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TPassEnd Yellow PPeppermint
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TSpecial Gingerbreadman
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TLicorice Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TSpecial Lollipop
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TSpecial Popsicle
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TSpecial Chocolate
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRegular Red
  , TRegular Purple
  , TRegular Yellow
  , TRegular Blue
  , TRegular Orange
  , TRegular Green
  , TRainbow
  ]

gameBoard = mkBoard tiles

type Deck = [Card]

standardDeck = singleCards ++ doubleCards1 ++ doubleCards2 ++ specialCards
  where
    singleCards = take 36 $ cycle $ map CSingle colors
    doubleCards1 = take 16 $ cycle $ map CDouble [Red, Purple, Yellow, Blue]
    doubleCards2 = take 12 $ cycle $ map CDouble [Orange, Green]
    specialCards = map CSpecial foods

shuffle :: Deck -> Deck
shuffle = id -- a placeholder function

data PlayerProgress
  = PlayerProgress 
  { pPosition :: Int
  , pWaits :: Int
  } deriving Show

initPlayer :: PlayerProgress
initPlayer = PlayerProgress { pPosition=0, pWaits=0 }

data Game
  = Game 
  { gProgress :: M.Map Player PlayerProgress
  , gState :: GameState
  , gDeck :: Deck
  , gTurns :: [Player]
  } deriving Show

data GameState
  = GContinue
  | GWonBy Player
  | GTerminated String
  deriving Show

terminate :: String -> S.StateT Game (R.ReaderT Board IO) ()
terminate err = S.get >>= \g -> S.put $ g { gState=GTerminated err }

winner :: Player -> S.StateT Game (R.ReaderT Board IO) ()
winner p = S.get >>= \g -> S.put $ g { gState=GWonBy p }

drawCard :: S.StateT Game (R.ReaderT Board IO) (Maybe Card)
drawCard = do
  g <- S.get
  case gDeck g of
    [] -> return Nothing
    (c:cs) -> do
      S.put $ g { gDeck=cs }
      return (Just c)

logGame :: String -> S.StateT Game (R.ReaderT Board IO) ()
logGame s = lift $ lift $ putStrLn s

turn :: S.StateT Game (R.ReaderT Board IO) ()
turn = do
  g <- S.get
  case gState g of
    GContinue -> case gDeck g of 
      [] -> terminate "out of cards!"
      (c:cards) -> case gTurns g of
        [] -> terminate "out of players! huh?"
        (p:players) -> case M.lookup p (gProgress g) of
          Nothing -> terminate "Who is this? How did the game even started?"
          (Just (PlayerProgress pos wait)) -> if wait > 0
            then
            do
              logGame $ (show p) ++ " has to wait!"
              S.put $ g
                { gProgress=M.insert p (PlayerProgress pos (wait-1)) (gProgress g)
                , gDeck=cards
                , gTurns=players
                }
            else
            do
              mf <- lift $ move c
              board <- lift $ R.ask
              let pos' = mf pos
              logGame $ (show p) ++ " moves from " ++ show pos ++ " to " ++ show pos'
              S.put $ g
                { gProgress=M.insert p (PlayerProgress pos' 0) (gProgress g)
                , gDeck=cards
                , gTurns=players
                }
              if pos' == lastTile board
                then winner p
                else return ()
            
    -- if the game state is not continue, do nothing    
    _ -> return ()


startGame :: [Player] -> Deck -> Game
startGame players deck = 
  Game { gProgress=M.fromList $ zip players $ map (const initPlayer) players
       , gState=GContinue
       , gDeck=deck
       , gTurns=cycle players
       }

play :: Game -> (R.ReaderT Board IO) ()
play g = do
  (_, g') <- S.runStateT turn g
  case gState g' of
    GTerminated s -> lift $ putStrLn s
    GWonBy winner -> lift $ putStrLn $ "We got a winner!" ++ show winner
    GContinue -> play g' 

playCandyLand :: Board -> Deck -> [Player] -> IO ()
playCandyLand _ _ [] = putStrLn "cannot play without any players"
playCandyLand board deck players = do
  putStrLn "Starting Candy Land..."
  let 
    initGame = startGame players deck
  R.runReaderT (play initGame) board
    
