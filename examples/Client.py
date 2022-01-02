import socket

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print(socket.gethostname())
s.connect(("10.1.29.11",46000))
s.send(bytes("Hello from client","ascii"))
for i in range(0,20):
    msg=s.recv(4096)
    print(msg.decode("utf8"))
while True:
    s.recv(4096)
#s.close()