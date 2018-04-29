
Env:
1) install rpcbind

2) run : rpcinfo 
check whether portmapper running. If not, add service persmission to binary files with: 
sudo setcap 'cap_net_bind_service=+ep' ~/Networking/lab4/bin_udp_client/client
sudo setcap 'cap_net_bind_service=+ep' ~/Networking/lab4/bin_udp_client/server

Generate Stubs
	./udp : rpcgen -N get_file.x (-N:to support multi args)

Build:
	./udp :  make 

	two binary files be stored in: ./bin_udp_server/server  ./bin_udp_client/client

prepare test file: 
	./bin_udp_server/test.txt


Run:

	./bin_udp_server :  ./server 
	./bin_udp_client :  ./client localhost test.txt


CHECK:
	./bin_tcp_client , a "test.txt" file will be tranfered from server and stored locally.

