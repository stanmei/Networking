
Env:
1) install rpcbind

2) run : rpcinfo 
check whether portmapper running. If not, add service persmission to binary files with: 
sudo setcap 'cap_net_bind_service=+ep' ~/Networking/lab4/bin_tcp_client/client
sudo setcap 'cap_net_bind_service=+ep' ~/Networking/lab4/bin_tcp_client/server

Generate Stubs
	./tcp : rpcgen get_file.x

Build:
	./tcp :  make 

	two binary files be stored in: ./bin_tcp_server/server  ./bin_tcp_client/client

prepare test file: 
	./bin_tcp_server/test.txt


Run:

	./bin_tcp_server :  ./server 
	./bin_tcp_client :  ./client localhost test.txt


CHECK:
	./bin_tcp_client , a "test.txt" file will be tranfered from server and stored locally.

