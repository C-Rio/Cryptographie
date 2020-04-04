all: main

main : main.cpp
	g++ main.cpp -o main -lssl -lcrypto
	
run : main
	./main
	
clean :
	rm main *.pem