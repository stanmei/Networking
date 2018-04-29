
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int filesize (char* file_name) ;

char* tx_buffer;
//file content pointer
char** get_file_content_1_svc (char* filename) {

        // seek file size
        int file_size = filesize(filename);
        // malloc memory space for file
        tx_buffer = malloc((file_size+1) * sizeof(*tx_buffer));
        if ( tx_buffer==NULL ) {
                printf ("[server] Failed to malloc for tx buffer\n");
                return NULL ;
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
	else {
		printf("[server] Read file bytes:%d.\n",num_byts);
	}

	tx_buffer[num_byts] ='\0';
        return &tx_buffer;

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


