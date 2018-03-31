/* File: server.c
 *
 * Author : grp1
 * 
 * Description : Server of distribute healthcare system
 *				 ./server <port>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 

#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 

#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h> 

#include <cJSON.h>
#include <sqldbop.h>
#include <clientopeartion.h>

#define MAX_PTHREADS_NUM 128
#define LISTEN_QUE_LEN 32

void* ClientOperation(void *argument);

int main (int argc, char* argv[]) {
	
	if ( argc !=2 ) {
		//userhelp();
		printf ("Abnormal arguments!\n");
		exit(-1);
	}
	
	int server_port = atoi(argv[1]) ;
	printf ("Listening on port:%d \n",server_port);
	
	// server socket
	int server_sock , accept_sock ;
	int ret ;

	struct sockaddr_in server; 	
	server.sin_family = AF_INET;
	server.sin_port = server_port;
	server.sin_addr.s_addr = INADDR_ANY;
	
	server_sock = socket(PF_INET,SOCK_STREAM,0);
	if ( server_sock < 0 ) {
		printf ("Failed to create server socket!");
		exit(-1);
	}
	
	ret = bind(server_sock,(struct sockaddr*) &server,sizeof(server)) ;
	if (ret<0) {
		printf ("Failed to bind server socket");
		close(server_sock);
		exit(-1);
	} 
	printf ("Bind to server_sock: %d\n",server_sock);
	
	// Initialize SQL server
	CreateDb() ;
	CreateUserTbl() ;

	//listening and accept client commands with individule thread
	pthread_t srv_pthreads [MAX_PTHREADS_NUM] ={0};
	int pth_idx = 0;
	
	while (1) {
		// listening client connection
		ret = listen(server_sock,LISTEN_QUE_LEN);
		if (ret<0) {
			printf ("Failed to listen on port!\n");
			close(server_sock);
			exit(-1);
		}
		printf("Listening on server_sock : %d \n",server_sock);
		//accept clients commands
		struct sockaddr_in client;
		memset(&client,0,sizeof(client));
		socklen_t client_addrlen = sizeof(client);
		
		accept_sock = accept (server_sock,(struct sockaddr*) &client,&client_addrlen);
		if (accept_sock <0) {
			printf ("Failed to accept from server!\n");
			close(server_sock);
			exit(-1);
		}
		
		//pthread to deal with client requests		
		if ( pthread_create(&srv_pthreads[pth_idx++],NULL,&ClientOperation,(void*)&accept_sock) <0) {
			printf ("Failed to create pthread!\n");
			close (server_sock);
			exit(-1);
		}
		
	}
	
	
	return 0;
}