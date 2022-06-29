import client1
import client2

if __name__ == '__main__':
    print(client1.get_global()) # prints by client2
    print(client2.get_global()) # prints by client2

    client1.init_mod()
    print(client2.get_global()) # prints by client1
