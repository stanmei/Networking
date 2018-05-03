
Env:
1) windows 8.1
2) ubuntu 16.04 running server.
3) Mysql on ubuntu.

Build:
        Eclipse

	one binary files be stored in: client

Run:

	(ubuntu:./server :  ./server <port>)
	
	windows cmd terminal:
	./client :  ./client <server address> <port>


Check:

This could though two ways:
1) ubuntu running server  -  rlogin mysql:  
   use myhealthdb;
   Then use mysql commands to show tables status.

2) windows running client - "query" command option in "client".

