import select
import socket

def main() -> None:
    host = socket.gethostname()
    port = 12345

    # create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.setblocking(0)
        sock.bind((host, port))
        sock.listen(5)
        print("server(nb) started...")

        # sockets from which we expect to read
        inputs = [sock]
        outputs = []

        while inputs:
            readable, writable, exceptional = select.select(
                inputs, outputs, inputs) # never time out

            # select() returns three new lists, containing subsets of the 
            # contents of the lists passed in.

            # All of the sockets in the readable list have incoming data 
            # buffered and available to be read. 

            # All of the sockets in the writable list have free space in their
            # buffer and can be written to. 

            # The sockets returned in exceptional have had an error, the actual
            # definition of exceptional condition depending on the platform.

            # The readable sockets represent three possible cases:

            # Case 1:
            # If the socket is the main server socket, the one being used to 
            # listen for connections, then the readable condition means it is 
            # ready to accept another incoming connection. In addition to 
            # adding the new connection to the list of inputs to monitor, you 
            # can set the client socket to not block. 

            # Case 2:
            # the next case is an established connection with a client that 
            # has sent data. The data is read with recv(), then placed on the
            # queue so it can be sent through the socket and back to the 
            # client.

            # Case 3:
            # A readable socket without data available is from a client that 
            # has disconnected, and the stream is ready to be closed

            # There are fewer cases for the writable connections. If there is 
            # data in the queue for a connection, the next message is sent. 
            # Otherwise, the connection is removed from the list of output 
            # connections so that the next time through the loop select() does
            # not indicate that the socket is ready to send data. 


            for r in readable:
                if r is sock:
                    conn, addr = r.accept()
                    inputs.append(conn) # a new 
                else: 
                    data = r.recv(1024)
                    if data:
                        print(data)
                    else:
                        inputs.remove(r) # disconnected
                        r.close()

# Here the server does not wait for all the data to be written to the buffer.
# When we make a socket non-blocking by calling setblocking(0), it will never
# wait for the operation to be completed. So when we call the recv method, it
# will return to the main thread. The main mechanical difference is that send,
# recv, connect and accept can return without doing anything at all.

# With this approach, we can perform multiple I/O operations with different 
# sockets from the same thread concurrently. But since we don't know if a 
# socket is ready for an I/O operation, we would have to ask each socket with 
# the same question and essentially spin in an infinite loop. 

# This non-blocking but still asynchronous approach is called I/O multiplexing.
# To get rid of this inefficient loop, we need polling readiness mechanism. 
# In this mechanism, we could interrogate the readiness of all sockets, and 
# they would tell us which one is ready for the new I/O operation and which one
# is not without beging explicitly asked. When any of the sockets is ready, we
# will perform operations in the queue and then be able to return to the 
# blocking state, waiting for any socket to be ready for the next I/O operation

if __name__ == '__main__':
    main()
