# receiving messages that contains timestamps that are increasing
# design an algorithm to drop messages that come too often

# given parameters like every (period) sec, no more than (mlimit) messages

class SpeedChecker(object):

    def __init__(self, period, mlimit):
        self.period = period
        self.mlimit = mlimit
        self.data = []

    def on_message(m):
        while self.data and m - self.data[0] > self.period:
            self.data.pop(0)

        if not self.data:
            self.data.append(m)

        else:
            if len(self.data) == self.mlimit:
                return False # m is dropped

            else:
                self.data.append(m)

        return True

                
