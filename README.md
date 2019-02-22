# Connect 4 - C/C++ Game
This is a [Connect4](https://en.wikipedia.org/wiki/Connect_Four) game I've done as a project for Computer Networks.

This application use TCP/IP communication protocol.

The game can be played by many players at the same time, due to the concurrent TCP/IP server.

To be able to play it you have to own Linux as an operating system.

# How to compile:
  * To compile just type **make** in your terminal to compile all files from current directory. Use **make clean** in your terminal to remove old compiled files. By default, the output files will be **server** for server.cpp file, and **client** for the client.cpp file.
  * If you want to compile file by file use 
    - **g++ server.cpp -o name1.out**
    - **g++ client.cpp -o name2.out**
	

# How to run:
  - firstly, you need to start the server: **./name1.out**
  - then you have  to connect minimum 2 players by using the following command : **./name2.out < IP > < PORT >** . By default, if you are running on localhost, the **IP** is **127.0.0.1** and the **PORT** is defined as **4444**
   > you can connect an unlimited number of players, but if the number of clients is odd, then the last connected player will be placed in a waiting queue until the opponent is connected
   
# How to play!
  - You'll be trained by the terminal information.
  
