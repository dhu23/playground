import socket
import sys
import time


def main() -> None:
    host = socket.gethostname()
    port = 12345

    # create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))
        print('connected to server')
        i = 0
        while True:
            data = str.encode(sys.argv[1])
            try:
                sock.send(data)
                print('%d: data sent => %s' % (i, data))
                i += 1
                time.sleep(2)
            except Exception as e:
                print(str(e))
                break

if __name__ == '__main__':
    assert len(sys.argv) > 1, "please provide message"
    main()
