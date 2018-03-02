/*******************************************************
 * Author : grp1
 * Desc   : tcp daytime client
 *          
 *          usage mode :
 *          1) client_tcp server port data
 *
 *	       Client send <data> to server; 
 *             Server response sqrt <data> to client.
 *             Client print both data.
 *
 *******************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <math.h>

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

#define PROTOCOL_TYP 0  //daytime server protocol (tcp:0,udp:1)

//user help information
void userhelp (void) ;
unsigned int ack_from_server (char* server_ip,char* msg,int protocol,int port);

int main (int argc, char* argv[]) {
	// Input arguments check
	if ( argc  < 2)  {
		printf ("Too Few arguments!\n");
		userhelp();
		exit(-1);
	}

	if ( argc  > 4)  {
		printf ("Too Many arguments!\n");
		userhelp();
		exit(-1);
	}

	int port = atoi(argv[2]);
	char* msg= argv[3]; 	

	// Arrary for read daytime,maxmum 2
	double read_result=0;
	int protocol = PROTOCOL_TYP ; 

	read_result = ack_from_server(argv[1],msg,protocol,port) ;
	printf ("Original Input Data : %d ; Result from server : %f ; \n",atoi(msg),read_result);

	return 0 ;

}

void userhelp () {
	printf ("usage: \n");
	printf ("client_tcp server port data\n");
}

unsigned int ack_from_server(char* server_ip,char* msg,int protocol,int port) { 
	/* Read daytime from server 
	 * 1) <gethostbyname> Get server ip hostnet structure.
	 * 2) socket
	 * 3) connect;
	 * 4) <write>send messages
	 * 5) <read> 
	 * 6) <close>
	 */

	//gethostbyname 
	struct hostent* server = gethostbyname(server_ip);

	if ( server== NULL ) {
		printf ("gethostbyname return unexpected null!");
		exit (-1);
	}

	//initial server addr for client socket
	//sockaddr_in : short sin_family ; short port ; struct addr 
	struct sockaddr_in server_addr ;
	memset (&server_addr,0,sizeof(server_addr)) ;
	
	server_addr.sin_family = AF_INET ;
	server_addr.sin_port   = htons(port);//PROTOCOL_PORT) ;
	memcpy (&server_addr.sin_addr,server->h_addr,server->h_length);

	//creat client socket
	int client_sock = socket(AF_INET,SOCK_STREAM,0);
	if ( client_sock == -1 ) {
		printf ("Fail to create client socket.\n");
		exit (-1);
	}
	printf("Creating client socket, protocol %d, %d......\n",protocol,port);

	if (protocol == PROTOCOL_TCP ) {
		//connect client with server.
		printf("Connecting client socket with %s,%d.....\n",server->h_name,ntohs(server_addr.sin_port));
		int client_cnnt = connect (client_sock,(struct sockaddr*)&server_addr,sizeof(server_addr));
		if ( client_cnnt== -1) {
			printf ("Fail to connect with server %d \n",client_cnnt);
		}
	}

	//send message to server
	//char* msg="Asking for daytime service"; 	
	ssize_t size_tmsg = sendto (client_sock,msg,strlen(msg),0,(struct sockaddr*)&server_addr,(socklen_t)sizeof(struct sockaddr));
	if ( size_tmsg==-1 ) {
		printf ("Fail to send message to server!\n");
	}
	printf("Sending client message to server %s......\n",server->h_name);

	//receive daytime from server
	socklen_t size_rcv = sizeof(server_addr);
	unsigned int daytime_rcv = sizeof(server_addr);
	ssize_t size_rmsg = recvfrom (client_sock,(char*)&daytime_rcv,sizeof(daytime_rcv),0,(struct sockaddr*)&server_addr,&size_rcv);
//	int size_rmsg = read(client_sock,(char*)&daytime_rcv,sizeof(daytime_rcv));

	if (size_rmsg==-1) {
		printf ("Fail to receive message from server!\n");
	}
	printf("Received  message from server %s , time: %x......\n",server->h_name,daytime_rcv);

	close(client_sock);
	return ntohl(daytime_rcv);

}
