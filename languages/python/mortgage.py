#!/opt/swt/bin/python

def calc_month_payment(original_balance, month_rate, term):
    tmp = (1+month_rate)**term
    return original_balance*month_rate*tmp/(tmp-1)

def calc_total_payment(original_balance, month_rate, term):
    return term*calc_month_payment(original_balance, month_rate, term)
