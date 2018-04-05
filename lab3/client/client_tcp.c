/*******************************************************
 * Author : grp1
 * Desc   : tcp  client
 *          
 *          usage mode :
 *          1) client_tcp server port filename
 *
 *	       Client send <file name> to server; 
 *             Server response <data of file> to client.
 *             Client print both data.
 *
 *******************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

#define PROTOCOL_TYP 0  //daytime server protocol (tcp:0,udp:1)

#define MAX_TRAN_SIZE 512 //max transition size
#define MAX_FILE_NAME_LEN 128 //max file name length
#define MAX_URL_LEN 128 // max url length

/*
 * function declartion
 */
//user help information
void userhelp (void) ;
unsigned int ack_from_server (char* server_ip,char* msg,int protocol,int port,char* file_name);

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

	//int msg= atoi(argv[3]); 	
	char* url_name= argv[3]; //lab3
	char* file_name ;
	
	if (url_name == NULL ) {
		printf ("Input filename is invalid!\n");
		exit(-1);
	}
	char* token ;
	token = strtok (url_name,"/") ;
	
	while (token!=NULL) {
		file_name = token;
		//printf ("User input file name: %s\n",file_name);
		token = strtok(NULL,"/");
	}
	printf ("User input file name: %s\n",file_name);
	// Arrary for read daytime,maxmum 2
	double read_result=0;
	int protocol = PROTOCOL_TYP ; 
	
	char http_request [MAX_URL_LEN] = {0};
	sprintf(http_request, "GET %s HTTP/1.0\r\n\r\n", url_name);

	read_result = ack_from_server(argv[1],http_request,protocol,port,file_name) ;
	printf ("Result from server : %f ; \n",read_result);

	return 0 ;

}

void userhelp () {
	printf ("usage: \n");
	printf ("client_tcp server port url\n");
}

unsigned int ack_from_server(char* server_ip,char* msg,int protocol,int port,char* file_name) { 
	/* Read result from server 
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
	//char* msg="Asking for service"; 	
	ssize_t size_tmsg = sendto (client_sock,msg,strlen(msg),0,(struct sockaddr*)&server_addr,(socklen_t)sizeof(struct sockaddr));
	if ( size_tmsg==-1 ) {
		printf ("Fail to send message to server!\n");
	}
	printf("Sending client message: %s, to server %s......\n",msg,server->h_name);

	/*
	* receive result from server,then write into local file
	*/

	//removed for lab3
	/*
	char file_name[MAX_FILE_NAME_LEN] ="rx_";
	strcat(file_name,msg);
	*/
	int ret = remove(file_name);
	if ( ret != -1) printf ("Removed existed local rx file.\n");

	char rx_buf[MAX_TRAN_SIZE] = {0} ;
	int fd = open(file_name,O_CREAT|O_WRONLY,S_IRWXU);
	//socklen_t size_rcv = sizeof(server_addr);
	//unsigned int daytime_rcv = sizeof(server_addr);
	//ssize_t size_rmsg = recvfrom (client_sock,(char*)&daytime_rcv,sizeof(daytime_rcv),0,(struct sockaddr*)&server_addr,&size_rcv);
	//int size_rmsg = read(client_sock,(char*)&daytime_rcv,sizeof(daytime_rcv));

	printf("Received file : %s contents from server %s ..\n",file_name,server->h_name);
	int total_rx =0;
	while (1) {

		int size_rmsg = read(client_sock,rx_buf,MAX_TRAN_SIZE);

		if (size_rmsg==-1) {
			printf ("Fail to receive message from server!\n");
			close(client_sock);
			close(fd);
			exit(-1);
		}
		if (size_rmsg == 0 ) break;

		int wsize_rx = write(fd,rx_buf,size_rmsg);
		if (wsize_rx != size_rmsg ) {
			printf ("Abnormal msg size when write receive message into file!\n");
			close(client_sock);
			close(fd);
			exit(-1);
		}	
		total_rx +=size_rmsg;
		//printf("contents: %s \n",rx_buf);
	}
	printf("Saved into local file : %s , received contents size: %d \n",file_name,total_rx);

	close(client_sock);
	close(fd);
	//return ntohl(daytime_rcv);
	return 0;

}
