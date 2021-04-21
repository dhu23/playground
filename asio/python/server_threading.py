import select
import threading
import socket


def handler(client):
    while True:
        data = client.recv(1024)
        if data:
            print(data)
    client.close()


def main() -> None:
    host = socket.gethostname()
    port = 12345

    # create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # bind the socket to the port
        sock.bind((host, port))
        # listen for incoming connections
        sock.listen(5)
        print("Server started...")

        while True:
            client, addr = sock.accept()
            threading.Thread(target=handler, args=(client,)).start()

    # right now it doesn't end the thread after client disconnection

if __name__ == '__main__':
    main()
