#fisier folosit pentru compilarea serverului concurent & clientului TCP

all:
	g++ server.cpp -o server
	g++ client.cpp -o client
clean:
	rm -f *~ client server
run:
	xterm ./client 127.0.0.1 2024 &
	xterm ./client 127.0.0.1 2024 &
	xterm ./server &
