/*************************************************************************
 * author : grp 1
 * 
 * desc   : tcp server ,which response sqrt of client incoming data.
 *          usage :  ./server_tcp <port>
 *
*************************************************************************/

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <math.h>

#define QUESIZE 32  //listen queue size limits

// user help
void usehelp (void);
// client commands function: isqrt
//double client_cmd_function (int rcv); 

int main (int argc, char* argv[]) {

	// arguments check
	if ( argc !=2 ) {
		usehelp();
		exit(-1) ;
	}

	//server listening port
	int server_port = atoi (argv[1]);

	// Create server socket 
	struct sockaddr_in server ;
	memset (&server,0,sizeof(server));

	server.sin_family = AF_INET ;
	server.sin_port = htons(server_port);
	server.sin_addr.s_addr = INADDR_ANY ; //all interfaces

	int server_sock = socket(PF_INET,SOCK_STREAM,0) ;
	if (server_sock == -1 ) {
		printf ("Fail to create server socket!\n");
		exit(-1);
	}
	printf("Create server socket.\n");

	// Bind server socket 
	int bindsock = bind (server_sock,(struct sockaddr *) &server,sizeof(server));
	if ( bindsock!=0 ) {
		printf ("Fail to bind server socket!\n");
		close (server_sock);
		exit(-1);
	}
	printf("Bind server socket.\n");

	while (1) {
		// listening at server port
		printf ("Listening at server port %d ......\n",server_port);
		int listensock = listen (server_sock,QUESIZE);
		if ( listensock !=0 ) {
			printf ("Fail to listening at port %d",server_port);
			close (server_sock);
			exit(-1);
		}

		// accept connection 
		struct sockaddr_in client ;
		memset (&client,0,sizeof(client));

		socklen_t client_addrlen = sizeof(client) ;
		int accept_sock = accept(server_sock,(struct sockaddr*)&client,&client_addrlen);//sizeof(client));
		if (accept_sock<0){
			printf("Fail to accept client socket!\n");
			close (server_sock);
			exit(-1);
		}
		printf ("Accepted at server port %d ......\n",server_port);

		// Receive message from client ;
		int rcd = 0;
		int rx = read (accept_sock,&rcd,sizeof(rcd));
		if (rx<0){
			printf ("Failt to read from client!\n");
		}
		printf ("Received %d at server port %d ......\n",rcd,server_port);

		// operation : sqrt 
		int txd = sqrt( (double) rcd );//client_cmd_function ((double) rcd); 
		// Send result back to client 
		int tx = write (accept_sock,&txd,sizeof(txd));
		if (tx<0){
			printf ("Failt to read from client!\n");
			close(server_sock);
			close(accept_sock);
			exit(-1);
		}
		//printf ("Sent %d at server port %d ......\n",txd,server_port);

		close(accept_sock);

	}
	return 0;
}	


void usehelp (void) {
	printf ("usage:\n");
	printf ("	server_tcp <port> \n" );
}

/*
double client_cmd_function (int rcd){ 
	return sqrt (rcd);
}
*/
