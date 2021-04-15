# explain how you would design a personal financial manager like mint.com.
# This system would connect to your bank accounts, analyze your spending
# habits and make recommendations

# a normalized data would look like:

# user info TABLE
# user id
# user name
# user login
# user type (premium/basic etc)

# account TABLE (this table is as big as user info table but mostly read-only)
# account id - unique per user per institute per account
# user id
# institution id
# acount type (credit/debt or bank/investment/equity account)
# account num
# credentials/password

# institution TABLE (this table is not so big, mostly read-only)
# institution id
# institution name
# connection method
# security info

# transaction TABLE
# user id
# account id
# transaction type (cash flow in or out)
# amount

# business questions to answer:
# 1, show equity/debt breakdown
# 2, show monthly cash inflows and cash outflows
# 3, show monthly "spending", including cash outflows and credit card spending
# 4, how to update data

# answer to 1 and 2
# for fast aggregation, it is probably necessary to have a monthly report
# stored for each user. And the current month should also be stored, and get
# updated with each login. Ledgers are kept for record but are not used
# often for real time user comsumption. It is too slow to go through all of
# the transactions(because it invovles database reads)

# answer to 4)
# each login and refresh button click would update data through connecting
# to financial institutions. 
