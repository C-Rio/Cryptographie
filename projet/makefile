CC = g++
CCFLAGS = -Wall -std=gnu++0x -lssl -lcrypto

all : server client

server : server.cpp DES.cpp rsa.cpp permutation.cpp socket.h
	@$(CC) server.cpp  DES.cpp rsa.cpp permutation.cpp $(CCFLAGS) -o server

client : client.cpp DES.cpp rsa.cpp permutation.cpp socket.h
	@$(CC) client.cpp  DES.cpp rsa.cpp permutation.cpp $(CCFLAGS) -o client

clean:
	rm -rf *.pem
	rm -f server client