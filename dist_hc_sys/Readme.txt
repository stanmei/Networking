Env:
1) ubuntu 16.04
2) Mysql on ubuntu 16.04.

Build:
	./lib/cjson:  make 
	./server   :  make 
	./client   :  make 

	two binary files be stored in: ./server/server  ./client/client

Run:

	./server :  ./server <port> 
	./client :  ./client <server address> <port>


Check:

This could though two ways:
1) rlogin mysql:  
   use myhealthdb;
   Then use mysql commands to show tables status.

2) "query" command option in "client".

