
/*************************************************************************
 * author : grp 1 
 * 
 * desc   : udp pre-threaded concurrent server 
 *
 *          usage :  ./server_udp <port>
 *
 *          A basic flow for udp connectionless server was as:
 *          create sock -> bind -> recv -> send -> close -> (recv) 
 *
*************************************************************************/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 

#include <unistd.h> 
#include <fcntl.h> 
#include <pthread.h> 
#include <errno.h>

#include <netdb.h> 
#include <netinet/in.h> 

#define QUESIZE 32  //listen queue size limits

#define MAX_FILE_NAME_LEN 128 //max file name character length
#define MAX_TX_SIZE 512 //max transmit size
#define MAX_THREADS_NUM 16

// user help
void usehelp (void);
// client commands function: isqrt
//double client_cmd_function (int rcv); 
int filesize (char* file_name) ;
int sendfile (int tx_sock,char* tx_file_name,int tran_byt_size,struct sockaddr_in* client,socklen_t client_addlen) ;

void* connectclient(void* arguments);

int main (int argc, char* argv[]) {

	// arguments check
	if ( argc !=2 ) {
		usehelp();
		exit(-1) ;
	}

	printf("-----------Pre threaded connection-oriented(tcp) server------------------\n");

	//server listening port
	int server_port = atoi (argv[1]);

	// Create server socket 
	struct sockaddr_in server ;
	memset (&server,0,sizeof(server));

	server.sin_family = AF_INET ;
	server.sin_port = htons(server_port);
	server.sin_addr.s_addr = INADDR_ANY ; //all interfaces

	int server_sock = socket(PF_INET,SOCK_DGRAM,0) ;
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

	//pthreads
	pthread_t accept_threads [MAX_THREADS_NUM] = {0};
	int thread_idx = 0;
	//struct args_stru args ;
	/*
	 * Child threads for file transfer with clients
	 */

	for (thread_idx=0;thread_idx<MAX_THREADS_NUM;thread_idx++) {
        	if (pthread_create(&accept_threads[thread_idx], NULL, &connectclient, (void *)&server_sock) != 0) {
       			printf("Fail to creat thread for accept socket!\n");
	       		return -1;
		}
	}

	pause();
	return 0;
}	


void usehelp (void) {
	printf ("usage:\n");
	printf ("	server_tcp <port> \n" );
}

int sendfile (int tx_sock,char* tx_file_name,int tran_byt_size,struct sockaddr_in* client,socklen_t client_addlen) {

	// seek file size
	int file_size = filesize(tx_file_name);
	// malloc memory space for file
	char* tx_buffer = malloc((file_size+1) * sizeof(*tx_buffer));
	if ( tx_buffer==NULL ) {
		printf ("Failed to malloc for tx buffer\n");
		return -1 ;
	}

	// open file 
	int fd = open(tx_file_name,O_RDONLY);	
	if ( fd < 0 ) {
		printf ("Fail to open file: %s;\n",tx_file_name);

		//free malloc
		free(tx_buffer);
		close(fd);
		exit(-1);
	}

	// read file conent into buffer
	int num_byts = read(fd,tx_buffer,(file_size+1));//sizeof(tx_buffer));
	if ( num_byts < 0 ) {
		printf ("Fail to read file contents;\n");

		//free malloc
		free(tx_buffer);
		close(fd);
		exit(-1);
	}

	// Loop to tx buffer content
	char* fp_cur = tx_buffer ; //current file pointer
	char* fp_end = fp_cur + num_byts -1 ; //end of file pointer
	int tx_tran_len = 0 ;

	printf("file content (filesize:%d; content size:%d bytes):\n",file_size,num_byts);
	//printf("%s",tx_buffer);

	int send_nums;
	while ( fp_cur < fp_end) {

		// Check transition size
		if ( fp_end - fp_cur < tran_byt_size) {
			tx_tran_len = fp_end - fp_cur + 1;	
		} else {
			tx_tran_len = tran_byt_size ;
		}

		// write tran len into sock
		//int send_nums = write(tx_sock,fp_cur,tx_tran_len);
		send_nums = sendto(tx_sock,fp_cur,tx_tran_len,0,(struct sockaddr*) client,client_addlen);
		if ( send_nums != tx_tran_len) {
			printf ("Abnormal write length. \n");

			//free malloc
			free(tx_buffer);
			close(fd);
			exit(-1);
		}
		fp_cur +=tx_tran_len;
	}
	send_nums = sendto(tx_sock,fp_cur,0,0,(struct sockaddr*) client,client_addlen); //udp
	//free malloc
	free(tx_buffer);
	close(fd);
	
	return 0;

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

void* connectclient(void* arguments) {
	int server_sock = *((int*) arguments) ;
	// accept connection 
	struct sockaddr_in client ;
	memset (&client,0,sizeof(client));

	socklen_t client_addrlen = sizeof(client) ;

	pthread_mutex_t lock ;
 	pthread_mutex_init(&lock, 0);

	while (1) {
		char tx_file_name [MAX_FILE_NAME_LEN]= {0} ;
		//mutex acquirin}
		pthread_mutex_lock (&lock);
		int rx = recvfrom (server_sock,tx_file_name,MAX_FILE_NAME_LEN, 0, (struct sockaddr*)&client, &client_addrlen);
		if (rx<0){
			printf ("Failt to send to client: %s!\n",strerror(errno));
			close(server_sock);
			exit(-1);
		}		
		pthread_mutex_unlock (&lock);

		//sendfile2client(&accept_sock);
		int tx = sendfile (server_sock,tx_file_name,MAX_TX_SIZE,&client,client_addrlen);
		if (tx<0){
			printf ("Failt to send to client!\n");
			close(server_sock);
			exit(-1);
		}
	}
	return NULL;
}
