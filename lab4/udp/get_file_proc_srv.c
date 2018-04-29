
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUF_SIZE 256

int filesize (char* file_name) ;

char* tx_buffer;
//char  buf_tran[MAX_BUF_SIZE];
char*  buf_tran;
//file content pointer
//char** get_file_content_1_svc (char* filename) {
int* get_file_trans_1_svc (char* filename) {

	/* notes: use static to avoid unexisted pointer to local var in client
	 */
	static int max_seq;
        // seek file size
        int file_size = filesize(filename);

        // malloc memory space for file
	if(filename!=NULL) { //check wether original existed, free before new allocation.
		free(tx_buffer);
		tx_buffer=NULL;
	}

        tx_buffer = malloc((file_size+1) * sizeof(*tx_buffer));
        if ( tx_buffer==NULL ) {
                printf ("[server] Failed to malloc for tx buffer\n");
                return 0 ;
        }

        // open file 
        int fd = open(filename,O_RDONLY);
        if ( fd < 0 ) {
                printf ("[Server] fail to open file: %s;\n",filename);

                //free malloc
                free(tx_buffer);
                close(fd);
                exit(-1);
        }

        // read file conent into buffer
        int num_byts = read(fd,tx_buffer,(file_size+1));//sizeof(tx_buffer));
        if ( num_byts < 0 ) {
                printf ("[server] Fail to read file contents;\n");
                //free malloc
                free(tx_buffer);
                close(fd);
                exit(-1);
        } 

	max_seq= ((int)ceil((double)file_size/MAX_BUF_SIZE));
	if(num_byts>=0) {
		printf("[server] Read file bytes:%d. Seqs num:%d.\n",num_byts,max_seq);
	}

	//return trans number
	return &max_seq;
	//return ((int)ceil((double)file_size/MAX_BUF_SIZE));
/*
	tx_buffer[num_byts] ='\0';
        return &tx_buffer;
*/

}

char** get_file_content_1_svc (char* filename,int seq) {
/*
char** get_file_content_1_svc (tran_seq* tran) {
	char* filename=tran.filename;
	int seq = tran.seq;
*/
	if(buf_tran!=NULL) { //check wether original existed, free before new allocation.
		free(buf_tran);
		buf_tran=NULL;
	}
        buf_tran = malloc((MAX_BUF_SIZE+1) * sizeof(*buf_tran));
	//printf("[server]Allocate seq buffer.\n");

	//pointer
	char* ptr= tx_buffer + MAX_BUF_SIZE*seq;

	strncpy(buf_tran,ptr,MAX_BUF_SIZE);
	//printf("[server]Return file content to client\n");
	return &buf_tran;
}

int filesize (char* file_name) {
        FILE* fp ;
        fp = fopen(file_name,"r");
        // Seek file size to transfer
        fseek(fp,0,SEEK_END);
        int size = ftell(fp); // get current pointer's position as file size.
        fseek(fp,0,SEEK_SET);

        //close file pointer
        fclose(fp);

        return size;
}


