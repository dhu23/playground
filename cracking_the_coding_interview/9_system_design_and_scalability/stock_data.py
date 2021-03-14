# imagine you are building some sort of service that will be called by up to
# 1000 client applications to get simple end-of-day stock price information
# (open, close, high, low). You may assume that you already have the data, and
# you can store it in any format you wish. How would you design the client
# facing service that provides the information to client applications?
# You are responsible for the development rollout, and ongoing monitoring
# and maintenance of the feed. Describe the different methods you considered
# why you would recommend your approach. Your service can use any technologies
# you wish and can distribute the information to the client applications in
# any mechanism you choose.


# Notes:
# stock price information on end of day basis. (20 year ~ 7000 data points)
# ~3000 stocks to track. (russell 3000)
# 7000 * 3000 = 21 million. It is possible to store the system in memory
# the point of the service is for fast feed
# user query: stock name and time range

# this is a front facing service so the interface would look like:
# a simplifed request:
#
# <xs:complexType name="StockReqInfo">
#   <xs:sequence>
#     <xs:element name="name" type="xs:string" />
#     <xs:element name="dates" type="xs:int"
#                 minOccur="0" maxOccur="unbounded" />
#     <xs:element name="priceType" type="xs:string"
#                 minOccur="0" maxOccur="unbounded" />
#   </xs:sequence>
# </xs:complexType>
#
# <xs:complexType name="DataPoint"
#   <xs:sequence>
#     <xs:element name="time" type="xs:int" />
#     <xs:element name="value" type="xs:double" />
#   </xs:sequence>
# </xs:complexType>
# 
# <xs:complexType name="TimeSeries"
#   <xs:sequence>
#     <xs:element name="series" type="DataPoint"
#                 minOccur="0" maxOccur="unbounded" />
#   </xs:sequence>
# </xs:complexType>
#
# <xs:complexType name="StockPriceSeries">
#   <xs:sequence>
#     <xs:element name="stock" type="xs:string" />
#     <xs:element name="prices" type="TimeSeries"
#                 minOccur="0" maxOccur="unbounded" />
#   </xs:sequence>
# </xs:complexType>
#
# <xs:complexType name="priceRequest">
#   <xs:sequence>
#     <xs:element name="stock" type="StockReqInfo"
#                 minOccur="0" maxOccur="unbounded" />
#   </xs:sequence>
# </xs:complexType>
#
# <xs:complexType name="priceResponse">
#   <xs:sequence>
#     <xs:element name="stockPrices" type="StockPriceSeries"
#                 minOccur="0" maxOccur="unbounded" />
#   </xs:sequence>
# </xs:complexType>
#

# load balancer to distribute 1000 clients' incoming requests
# there are probably two layers of load balancing that can happen
# level 1: general load balancing to distribute requests
# level 2: using specific domain logic to distribute requests based on stocks
# all services are identical
# some cache in front of the service will be needed to improve performance


# request generated -> sent to request router ->
# request is distributed to a dedicated machine with cache in front ->
# (can strip the requests by stock name and distribute request to the cluster)
# on cache miss, collect data from backend and put back together and send the
# response
