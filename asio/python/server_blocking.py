import socket

def main() -> None:
    host = socket.gethostname()
    port = 12345

    # create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # bind the socket to the port
        sock.bind((host, port))
        # listen for incoming connections
        sock.listen(5)
        print("server started...")

        while True:
            conn, addr = sock.accept() # blocking
            print('connected by ' + str(addr))
            while True:
                data = conn.recv(1024) # receiving data, blocking
                if not data:
                    break
                print(data)

if __name__ == '__main__':
    main()
    
