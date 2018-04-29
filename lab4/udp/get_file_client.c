
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <string.h>

#include <rpc/rpc.h>

#include "get_file.h"

//#define PROTOC udp 

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
	if ((cl=clnt_create(server,GETFILEPROG,GETFILE_VER,"udp"))==NULL) {
	// Fail to establish connection with server
	printf("Fail to establish connection with server %s\n",server);
	exit(-1);
	} else {
		printf("Create establish connection with server %s\n",server);
	}	

	// Call the remote procedure 
	// Write buffer into file	
	int fd;
	if (filename!=NULL) {	
		fd = open(filename,O_CREAT|O_WRONLY,S_IRWXU); 	
		if (fd<0) {
			printf("Fail to open file:%s\n",filename);
			exit(-1);
		}
	} 


	int max_seq=0;
	//printf("[client] Start to call file trans procedure,max_seq:%d.\n",max_seq);
	/*notice: this procedure introduced lots debugging efforts due:
	 *1) x file generted int* instead of expected int return.
         *2) int* return required "static" to avoid un-existed pointer to local variable in server side.
	 * 
	 */
	max_seq = *get_file_trans_1(filename,cl);

	int seq_rcv =0;
	//printf("[client] Start to call file content procedure,max_seq:%d,seq_rcv:%d.\n",max_seq,seq_rcv);
	//tran_seq tran={filename,seq_rcv};
	// UDP , write individual seq into file.
	while (seq_rcv < max_seq) {
		if (rx_file_content!=NULL) {
			free(*rx_file_content);
		}
		//tran.seq=seq_rcv;
		rx_file_content = get_file_content_1(filename,seq_rcv,cl);
		if(rx_file_content==NULL){
			printf("Fail to call the remote procedure.\n");	
			exit(-1);
		} 
/*
		else {
			printf("[client] received seq from remote procedeure\n");
		}
*/

		write(fd,*rx_file_content,(int) strlen(*rx_file_content));
		seq_rcv++;
	}

	close(fd);
	printf("Transfer done,close file.\n");
	// Destroy handle
	clnt_destroy(cl); 

	return 0;
}
