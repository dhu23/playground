[D3, d3] = det1(3, false)

D3prox = round(inv(D3))

D3prox*D3 == eye(3)
abs(d3) == 1


[D6, d6] = det1(6, false)

D6prox = round(inv(D6)) % floor or ceil won't be good

D6prox*D6 == eye(6)
abs(d6) == 1
