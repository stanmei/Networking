
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <string.h>

#include <rpc/rpc.h>

#include "get_file.h"

//#define PROTOC tcp 

int main(int argc,char* argv[]) {

	//client handl
	CLIENT *cl = NULL;
	// arguments
	char* server = NULL;
	char* filename = NULL;
	
	char** rx_file_content = NULL;

	// Input arguments number check.
	if ( argc!=3) {
		printf("usage: ./get_file_client <server> <filename>\n");	
		exit(-1);
	}

	server = argv[1];
	filename = argv[2];

	//remove local existed result file for a new trans
        int ret = remove(filename);
        if ( ret != -1) printf ("Removed existed local %s.\n",filename);

	// Create client handle
	if ((cl=clnt_create(server,GETFILEPROG,GETFILE_VER,"tcp"))==NULL) {
	// Fail to establish connection with server
	printf("Fail to establish connection with server %s\n",server);
	exit(-1);
	}	
	else {
	printf("Establish connection with server %s. \n",server);
	}
	// Call the remote procedure 
	rx_file_content = get_file_content_1(filename,cl);
	if(rx_file_content==NULL){
		printf("Fail to call the remote procedure.\n");	
		exit(-1);
	}
	else {
		printf("Received file context from server.\n");
	}

	// Write buffer into file	
	if (filename!=NULL) {	
		int fd = open(filename,O_CREAT|O_WRONLY,S_IRWXU); 	
		if (fd<0) {
			printf("Fail to open file:%s\n",filename);
			exit(-1);
		}
		write(fd,*rx_file_content,(int) strlen(*rx_file_content));
	} 

	// Destroy handle
	clnt_destroy(cl); 

	return 0;
}
