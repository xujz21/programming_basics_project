from socket import socket, AF_INET,SOCK_STREAM
from threading import Thread

class service:
    SERVER=socket(AF_INET,SOCK_STREAM)
    clients={}
    addresses={}
    BUFFER_SIZE=10000010


    def accept_connections(self)->None:
        '''Accept connection request from a berkeley socket'''
        while True:
            client_socket,client_address_port=self.SERVER.accept()
            print("%s:%s connected."%client_address_port)
            self.addresses[client_socket]=client_address_port
            Thread(target=self.read,args=(client_socket,)).start()
            
    def read(self,client:socket)->None:
        '''Handle a client, and read data from the network stream'''
        self.clients[client]=1
        self.WRITE(client,"Greetings from server")
        while True:
            try:
                msg=client.recv(self.BUFFER_SIZE).decode()
                if msg=="":
                    raise ConnectionAbortedError
                print(msg)
            except:
                if(client in self.clients):
                    print("%s:%s disconnected."%self.addresses[client])
                    client.close()
                    del self.clients[client]
                break
    @staticmethod
    def WRITE(client:socket,msg:str)->None:
        '''Write data to the network stream'''
        client.send(bytes(msg,"ascii"))
    
    def run(self):
        self.SERVER.bind(("",46000))
        self.SERVER.listen(5)
        #print(SERVER)
        ACCEPT_THREAD=Thread(target=self.accept_connections)
        ACCEPT_THREAD.start()
        #broadcast_signals()
        ACCEPT_THREAD.join()
        self.SERVER.close()

if __name__=="__main__":
    service().run()