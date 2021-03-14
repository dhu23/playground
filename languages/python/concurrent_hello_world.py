from queue import Queue

event_loop = Queue()


def do_hello():
    global event_loop
    print("Hello")
    event_loop.put(do_world)


def do_world():
    global event_loop
    print('world')
    event_loop.put(do_hello)


if __name__ == '__main__':
    event_loop.put(do_hello)

    while True:
        function = event_loop.get()
        function()
