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

#define PROTOCOL_PORT   37//13 //daytime server port (tcp:37)

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

#define PROTOCOL_TYP 1  //daytime server protocol (tcp:0,udp:1)

#define NTP_UNIX_TS_OFST 2208988800 //NTP timestamp offset to unix

//user help information
void userhelp (void) ;
unsigned int ack_from_server (char* server_ip,char* msg,int protocol);

int main (int argc, char* argv[]) {
	// Input arguments check
	if ( argc  < 2)  {
		printf ("Too Few arguments!\n");
		userhelp();
		exit(-1);
	}

	if ( argc  > 3)  {
		printf ("Too Many arguments!\n");
		userhelp();
		exit(-1);
	}

	char* msg="Asking for daytime service"; 	

	// Arrary for read daytime,maxmum 2
	long unsigned int read_daytime[2] ={0};
	int protocol = PROTOCOL_TYP ; 

	for (int idx=1;idx<argc;idx++) {
		// call daytime service
		read_daytime [idx-1] = ack_from_server(argv[idx],msg ,protocol) - NTP_UNIX_TS_OFST ;
		printf ("Time from server %d :%lx , %s \n",idx-1,read_daytime[idx-1],ctime((time_t*)&read_daytime[idx-1]));
		sleep(2);
	}

	if ( argc > 2 ) {
		printf ("\n");
		printf ("Time difference between servers : %lx s \n",read_daytime[1]-read_daytime[0]);
	}

	return 0 ;

}

void userhelp () {
	printf ("usage: \n");
	printf ("daytime_client_tcp server1 (server2) \n");
}

unsigned int ack_from_server(char* server_ip,char* msg,int protocol) { 
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
	server_addr.sin_port   = htons(PROTOCOL_PORT) ;
	memcpy (&server_addr.sin_addr,server->h_addr,server->h_length);

	//creat client socket
	int dgram_typ = SOCK_DGRAM ;
	if (protocol == PROTOCOL_TCP ) {
		dgram_typ = SOCK_STREAM;
	} 

	int client_sock = socket(AF_INET,dgram_typ,0);

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
	//char* msg="Asking for daytime service"; 	
	printf("Sending client message to server %s......\n",server->h_name);
	ssize_t size_tmsg = sendto (client_sock,msg,strlen(msg),0,(struct sockaddr*)&server_addr,(socklen_t)sizeof(struct sockaddr));
	if ( size_tmsg==-1 ) {
		printf ("Fail to send message to server!\n");
	}

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
