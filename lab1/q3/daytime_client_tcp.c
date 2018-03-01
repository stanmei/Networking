/*******************************************************
 * Author : grp1
 * Desc   : tcp daytime client
 *          
 *          usage mode :
 *          1) daytim_client server1
 *		read day time and print from server1
 *          2) daytim_client server1 server2
 *		read day time and print from server1/server2
 *		report time difference between two servers
 *
 * ****************************************************/
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define DAYTIME_PORT   37

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

#define DYATIME_PROTOCOL 0 

//user help information
void userhelp (void) ;
unsigned int acquire_daytime (char* server_ip,int protocol);

int main (int argc, char* argv[]) {
	// Input arguments check
	if ( argc  < 1)  {
		printf ("Too Few arguments!\n");
		userhelp();
		exit(-1);
	}

	if ( argc  > 2)  {
		printf ("Too Many arguments!\n");
		userhelp();
		exit(-1);
	}
	// Arrary for read daytime,maxmum 2
	unsigned int read_daytime[2] ={0};
	int protocol = DYATIME_PROTOCOL ; 


	for (int idx=1;idx<argc;idx++) {
		read_daytime [idx] = acquire_daytime(argv[idx] ,protocol);
	}

	printf ("Time:%x",read_daytime[0]);
	printf ("Time:%x",read_daytime[1]);
	printf ("Time difference:%x",read_daytime[0]-read_daytime[1]);

	return 0 ;

}

void userhelp () {
	printf ("usage: \n");
	printf ("daytime_client_tcp server1 (server2) \n");
}

unsigned int acquire_daytime(char* server_ip,int protocol) { 
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
	server_addr.sin_port   = htons(DAYTIME_PORT) ;
	memcpy (&server_addr.sin_addr,server->h_addr,server->h_length);

	//creat client socket
	int client_sock = socket(AF_INET,SOCK_STREAM,0);
	if ( client_sock == -1 ) {
		printf ("Fail to create client socket.\n");
		exit (-1);
	}
	printf("Creating client socket, protocol %d......\n",protocol);

	if (protocol == PROTOCOL_TCP ) {
		//connect client with server.
		printf("Connecting client socket with %s,%d.....\n",server->h_name,ntohs(server_addr.sin_port));
		int client_cnnt = connect (client_sock,(struct sockaddr*)&server_addr,sizeof(server_addr));
		if ( client_cnnt== -1) {
			printf ("Fail to connect with server %d \n",client_cnnt);
		}
	}

	//send message to server
	char* msg="Asking for daytime service"; 	
	ssize_t size_tmsg = sendto (client_sock,msg,strlen(msg),0,(struct sockaddr*)&server_addr,(socklen_t)sizeof(struct sockaddr));
	if ( size_tmsg==-1 ) {
		printf ("Fail to send message to server!\n");
	}
	printf("Sending client message to server %s......\n",server->h_addr);

	//receive daytime from server
	unsigned int daytime_rcv = 0 ;
	ssize_t size_rmsg = recvfrom (client_sock,(char*)&daytime_rcv,sizeof(daytime_rcv),0,(struct sockaddr*)&server_addr,(socklen_t*)&daytime_rcv);

	if (size_rmsg==-1) {
		printf ("Fail to receive message from server!\n");
	}
	printf("Received  message from server %s......\n",server->h_addr);

	close(client_sock);
	return daytime_rcv;

}
