/* File : connserver.c
 *
 * Author : grp1
 *
 * Desc : Connect with server via tcp. 
 *
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
#include <errno.h>


int ConnServer (struct hostent* server_ip,int server_port) {

	// build server socket
	struct sockaddr_in server;
	memset(&server,0,sizeof(server));
	
	server.sin_family= AF_INET; 
	server.sin_port= server_port;//htons(server_port);
	memcpy(&server.sin_addr,server_ip->h_addr,server_ip->h_length); //copy server ip into server structure.
	
	// client socket
	int client_sock = socket(AF_INET,SOCK_STREAM,0);
	if (client_sock<0) {
		printf ("Failed to create client socket------- error code:%s!\n",strerror(errno));
		return client_sock;
	}
	
	printf ("Created client socket: %d... \n",client_sock);
	printf ("Trying to connect with server: %s , port: %d ........... \n",server_ip->h_name, server_port);
	// Connect with server
	int ret ;
	ret = connect(client_sock,(struct sockaddr*) &server,sizeof(server));
	
	if (ret <0) {
		printf("Connection error code: %s!\n",strerror(errno));
		close(client_sock);
		return ret;
	}
	
	return client_sock;
}
