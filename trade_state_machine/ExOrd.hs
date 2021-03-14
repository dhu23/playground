module ExOrd where -- exchange order state


type OrdId = String
type RplOrdId = String
type CxlOrdId = String

type LmtPx = Double
type FillPx = Double
type OrdQty = Int
type FillQty = Int

type RejectReason = String
type ErrorReason = String

data TradeFill 
    = TradeFill 
    { tfQty :: FillQty 
    , tfPx :: FillPx 
    } deriving Show

type FillTrace = [TradeFill]

-- trace information of an exchange order, such as id, px/qty
data OrdInfo
    = OrdInfo
    { ordId :: OrdId
    , lmtPx :: LmtPx
    , ordQty :: OrdQty
    } deriving Show

type RplOrdInfo = OrdInfo

data ExOrdState 
    = SInitial
    | SPendingAck OrdInfo
    | SAtExchange OrdInfo FillTrace
    | SRejected OrdInfo
    | SPendingRpl OrdInfo FillTrace RplOrdInfo
    | SPendingCxl OrdInfo FillTrace CxlOrdId
    | SPendingPendingRplCxl OrdInfo FillTrace RplOrdInfo CxlOrdId
    | SPendingPendingAckCxl OrdInfo CxlOrdId
    | SPendingErrorOut OrdInfo FillTrace ErrorReason
    | SError OrdInfo FillTrace ErrorReason
    | SErrorOut OrdInfo FillTrace
    | SPartiallyOuted OrdInfo FillTrace
    | SOuted OrdInfo FillTrace
    | SDone OrdInfo FillTrace
    deriving Show


data ExchMsg 
    = Ack OrdId
    | Reject OrdId
    | RplAck OrdId RplOrdId
    | RplReject OrdId RplOrdId
    | CxlAck OrdId CxlOrdId
    | CxlRej OrdId CxlOrdId
    | Out OrdId
    | Fill OrdId TradeFill
    | PartialFill OrdId TradeFill
    deriving Show


-- receive incoming messages from the exchange
type Receive s e = s -> e -> IO s

receive :: Receive ExOrdState ExchMsg

-- PendingAck
receive (SPendingAck oi) (Ack oid)
  | ordId oi == oid = return $ SAtExchange oi []
  | otherwise = return $ SPendingAck oi

receive (SPendingAck oi) (Reject oid)
  | ordId oi == oid = return $ SRejected oi
  | otherwise = return $ SPendingAck oi

-- AtExchange
receive (SAtExchange oi fills) (Out oid) = return $ SOuted oi fills

receive (SAtExchange oi fills) (Fill oid tf)
  | ordId oi == oid = return $ SDone oi (tf:fills)
  | otherwise = return $ SAtExchange oi fills

receive (SAtExchange oi fills) (PartialFill oid tf)
  | ordId oi == oid = return $ SAtExchange oi (tf:fills)
  | otherwise = return $ SAtExchange oi fills

-- PendingRpl state
receive (SPendingRpl oi fills roi) (RplAck oid rid)
  | ordId oi == oid && ordId roi == rid = return $ SAtExchange roi fills
  | otherwise = return $ SPendingRpl oi fills roi

receive (SPendingRpl oi fills roi) (RplReject oid rid)
  | ordId oi == oid && ordId roi == rid = return $ SAtExchange oi fills
  | otherwise = return $ SPendingRpl oi fills roi

receive (SPendingRpl oi fills roi) (Out oid)
  | ordId oi == oid = return $ SOuted oi fills
  | otherwise = return $ SPendingRpl oi fills roi

receive (SPendingRpl oi fills roi) (Fill oid tf)
  | ordId oi == oid = return $ SDone oi (tf:fills)
  | otherwise = return $ SPendingRpl oi fills roi

receive (SPendingRpl oi fills roi) (PartialFill oid tf)
  | ordId oi == oid = return $ SPendingRpl oi (tf:fills) roi -- mark done??
  | otherwise = return $ SPendingRpl oi fills roi

-- PendingCxl state
receive (SPendingCxl oi fills cid) (CxlAck oid cid')
  | ordId oi == oid && cid == cid' = return $ SOuted oi fills
  | otherwise = return $ SPendingCxl oi fills cid

receive s _ = return s


data ExOrdAct
    = New OrdInfo
    | Rpl RplOrdInfo
    | Cxl CxlOrdId
    deriving Show


type Send s e = s -> e -> IO s

send :: Send ExOrdState ExOrdAct

-- Initial state
send SInitial (New oi) = return $ SPendingAck oi

-- PendingAck state
send (SPendingAck oi) (Cxl cid) 
  = return $ SPendingPendingAckCxl oi cid

-- AtExchange state
send (SAtExchange oi fills) (Rpl roi)
  = return $ SPendingRpl oi fills roi
  
send (SAtExchange oi fills) (Cxl cid)
  = return $ SPendingCxl oi fills cid

-- Rejected state is a terminal state

-- PendingRpl state
send (SPendingRpl oi fills roi) (Cxl cid)
  = return $ SPendingPendingRplCxl oi fills roi cid

-- PendingCxl state

send s _ = return s



--data ExOrdEvt 
--    = Send ExOrdAct
--    | Receive ExchMsg 
--    deriving Show
