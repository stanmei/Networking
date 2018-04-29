C       = gcc
INCLUDES = -I .
CFLAGS   = -g -Wall $(INCLUDES) -pthread
LDFLAGS  = -lm
LDPATH   = 

TARGET1  = client server 
#OBJ1      = client_tcp.o 

#define rpcgen files
RPC_FILES=get_file.h get_file_clnt.c get_file_svc.c get_file_xdr.c

all: ${RPC_FILES} ${TARGET1}

${RPC_FILES}:
	rpcgen -N get_file.x

# client 
client: get_file_client.o get_file_clnt.o
	$(CC) $(CFLAGS) -o ../bin_tcp_client/client get_file_client.o get_file_clnt.o

get_file_client.o:
	$(CC) $(CFLAGS) -c get_file_client.c

get_file_clnt.o:
	$(CC) $(CFLAGS) -c get_file_clnt.c

# server 
server: get_file_svc.o get_file_proc_srv.o
	$(CC) $(CFLAGS) -o ../bin_tcp_server/server get_file_proc_srv.o get_file_svc.o

get_file_svc.o:
	$(CC) $(CFLAGS) -c get_file_svc.c

get_file_proc_srv.o:
	$(CC) $(CFLAGS) -c get_file_proc_srv.c

clean:
		rm -rf *.o rx_*.txt $(TARGET1) 
