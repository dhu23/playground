class Stack(object):
    def __init__(self):
        self.data = []

    def push(self, x):
        self.data.append(x)

    def size(self):
        return len(self.data)

    def pop(self):
        if not self.data:
            raise Exception("Stack is empty")
        return self.data.pop(-1) # pop the last one


class QueueAsStacks(object):
    def __init__(self):
        self.s1 = Stack()
        self.s2 = Stack()

    def _transfer(self):
        if self.s2.size():
            raise Exception("Cannot transfer stack while s2 is not empty")
        for i in xrange(self.s1.size()):
            self.s2.push(self.s1.pop())

    def push(self, x):
        self.s1.push(x)

    def pop(self):
        if self.s2.size():
            return self.s2.pop()
        else:
            self._transfer()
            if self.s2.size():
                return self.s2.pop()
            else:
                raise Exception("Queue is empty")

if __name__ == "__main__":
    s = Stack()
    s.push(1)
    s.push(2)
    print s.pop()
    q = QueueAsStacks()
    q.push(1)
    print q.pop()
