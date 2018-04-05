/*************************************************************************
 * author : grp 1 
 * 
 * desc   : tcp iterative server 
 *
 *          usage :  ./server_tcp <port>
 *
 *          A basic flow for iterative tcp server was as:
 *          create sock -> bind -> listen -> 
 *          accept -> recv -> send -> close -> (accept) 
 *
*************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 

#include <unistd.h> 
#include <fcntl.h> 
#include <signal.h> 

#include <netdb.h> 
#include <netinet/in.h> 

#define QUESIZE 32  //listen queue size limits

#define MAX_FILE_NAME_LEN 128 //max file name character length
#define MAX_TX_SIZE 512 //max transmit size
#define NUM_HTTP_RESP_CODE 4

/*
 * Http default response header declration -- reuse from internet.
 */
// HTTP response status code
#define HTTP_SUCCESS_200                 200
#define HTTP_CLIENT_ERR_BAD_REQUEST      400
#define HTTP_CLIENT_ERR_FORBIDDEN        403
#define HTTP_CLIENT_ERR_FILE_NOT_FOUND   404

#define HTTP_HEADER_LEN               64 
#define HTTP_REQ_BUF_SIZE            512 

char g_bad_req_msg[] = "<!DOCTYPE HTML PUBLIC '-//IETF//DTD HTML 2.0//EN'> \r\n \
<html><head> \r\n \
<title>400 Bad Request</title> \r\n \
</head><body> \r\n \
<h1>400 Bad Request</h1> \r\n \
<p>Your browser sent a request that this server could not understand.<br /> \r\n \
</p> \r\n \
<hr> \r\n \
<address>CMPE207 Lab3 Server at 127.0.1.1 Port 80</address> \r\n \
</body></html> \r\n"; 

char g_forbidden_msg[] = "<!DOCTYPE HTML PUBLIC '-//IETF//DTD HTML 2.0//EN'> \r\n \
<html><head> \r\n \
<title>403 Forbidden</title> \r\n \
</head><body> \r\n \
<h1>403 Forbidden</h1> \r\n \
<p>You don't have permission to access /index.html on this server.<br /> \r\n \
</p> \r\n \
<hr> \r\n \
<address>CMPE-207 Server at localhost Port 80</address> \r\n \
</body></html> \r\n"; 

char g_not_found_msg[] =  "<!DOCTYPE HTML PUBLIC '-//IETF//DTD HTML 2.0//EN'> \r\n \
<html><head> \r\n \
<title>404 NOT FOUND</title> \r\n \
</head><body> \r\n \
<h1>404 NOT FOUND</h1> \r\n \
<p> \r\n \
Your browser sent a request that this server could not find on this server.<br /> \r\n \
</p> \r\n \
<hr> \r\n \
<address>CMPE207 Lab3 Server at localhost Port 80</address> \r\n \
</body></html> \r\n"; 

/*
 * HTTP response header structure
 */
typedef struct http_resp_cntx {
	int resp_sts_code ;
	const char* head_sts_str ;
	char* resp_err_body_str ;
} http_resp_cntx_t ;

http_resp_cntx_t g_ary_http_resp_cntx [] = {
	{ HTTP_SUCCESS_200,               "HTTP/1.0 200 OK\r\n\r\n", ""},
    { HTTP_CLIENT_ERR_BAD_REQUEST,    "HTTP/1.0 400 Bad Request\r\n\r\n", g_bad_req_msg},
    { HTTP_CLIENT_ERR_FORBIDDEN,      "HTTP/1.0 403 Forbidden\r\n\r\n",g_forbidden_msg},
    { HTTP_CLIENT_ERR_FILE_NOT_FOUND, "HTTP/1.0 404 Not Found\r\n\r\n",g_not_found_msg}
};

/*
 * Function declartion
 */
// user help
void usehelp (void);
// client commands function: isqrt
//double client_cmd_function (int rcv); 
int filesize (char* file_name) ;
int sendfile (int tx_sock,char* tx_file_name,int tran_byt_size,int http_sts_code) ;

int parse_tx_file_name (char* tx_file_name,char* rcv_http_req_msg);

void childshandle(int signal);

int main (int argc, char* argv[]) {

	// arguments check
	if ( argc !=2 ) {
		usehelp();
		exit(-1) ;
	}

	printf("-----------Multiprocessor connection-oriented(tcp) server------------------\n");
	char tx_file_name [MAX_FILE_NAME_LEN]= {0} ;
	char rcv_http_req_msg [HTTP_HEADER_LEN]= {0} ;

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

	// listening at server port
	printf ("Listening at server port %d ......\n",server_port);
	int listensock = listen (server_sock,QUESIZE);
	if ( listensock !=0 ) {
		printf ("Fail to listening at port %d",server_port);
		close (server_sock);
		exit(-1);
	}

	while (1) {
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

		/*
		 * Child process for file transfer with clients
		 */
		signal(SIGCHLD,childshandle);

		int child = fork();
		if ( child==0 ) { // child process 
			printf ("chld process:%d.\n",(int)getpid());
			// Receive message from clienBindt ;
			//int rcd = 0;
			int rx = read (accept_sock,&rcv_http_req_msg,MAX_FILE_NAME_LEN);//sizeof(rcd));
			if (rx<0){
				printf ("Failt to read from client!\n");
			}	
	
			int http_sts_code = parse_tx_file_name (tx_file_name,rcv_http_req_msg) ;				
			
			// Send result back to client 			
			//int tx = write (accept_sock,&txd,sizeof(txd));
			printf ("Received at server port %d , required file name: %s, sts code: %d .....,\n",server_port,tx_file_name,http_sts_code);	
			int tx = sendfile (accept_sock,tx_file_name,MAX_TX_SIZE,http_sts_code);					
			if (tx<0){
				printf ("Failt to send to client!\n");
				close(server_sock);
				close(accept_sock);
				exit(-1);
			}
			//printf ("Sent %d at server port %d ......\n",txd,server_port);
			close(accept_sock); //child process finish
			exit(0);
		}
		//parent waiting for childs
		pid_t pid;
    	int state;
    	while ((pid = waitpid(-1, &state, WNOHANG)) > 0){
       	 	printf("child_pid: %d finished!\n", pid);
        }

		close(accept_sock); //parent process finish

	}
	return 0;
}	


void usehelp (void) {
	printf ("usage:\n");
	printf ("	server_tcp <port> \n" );
}

int sendfile (int tx_sock,char* tx_file_name,int tran_byt_size,int http_sts_code) {
	int file_size =0 ;
	int send_nums ;
	
	/*
	 * Abnormal http response
	 */
	printf ("Received file name: %s , http resp code: %d.\n",tx_file_name,http_sts_code);
	
	int sts_idx = -1;

	for (;sts_idx<NUM_HTTP_RESP_CODE;sts_idx++)
		if (g_ary_http_resp_cntx[sts_idx].resp_sts_code==http_sts_code) break;
			
	send_nums = write(tx_sock,g_ary_http_resp_cntx[sts_idx].head_sts_str,strlen(g_ary_http_resp_cntx[sts_idx].head_sts_str));
	if ( g_ary_http_resp_cntx[sts_idx].resp_sts_code!=HTTP_SUCCESS_200){		
		send_nums = write(tx_sock,g_ary_http_resp_cntx[sts_idx].resp_err_body_str,strlen(g_ary_http_resp_cntx[sts_idx].resp_err_body_str));
		printf ("Unsuccessful http request, retrun.\n");
		return 0;
	}

	/*
	 * Send local file.
	 */
	// seek file size
	file_size = filesize(tx_file_name);
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

	while ( fp_cur < fp_end) {

		// Check transition size
		if ( fp_end - fp_cur < tran_byt_size) {
			tx_tran_len = fp_end - fp_cur + 1;	
		} else {
			tx_tran_len = tran_byt_size ;
		}

		// write tran len into sock
		send_nums = write(tx_sock,fp_cur,tx_tran_len);
		if ( send_nums != tx_tran_len) {
			printf ("Abnormal write length. \n");

			//free malloc
			free(tx_buffer);
			close(fd);
			exit(-1);
		}
		fp_cur +=tx_tran_len;
	}

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

void childshandle(int signal){
	pid_t pid;
	int state;
	while ( (pid=waitpid(-1,&state,WNOHANG))>0) 
		printf ("pid %d finished\n",pid);

}

int parse_tx_file_name (char* tx_file_name,char* rcv_http_req_msg){
	char* elem = NULL;
	
	// First element : GET
	elem = strtok(rcv_http_req_msg," ");
	printf ("First extracted element from http_req:%s , from : %s;\n",elem,rcv_http_req_msg);
	if ( elem == NULL ) {
		return HTTP_CLIENT_ERR_BAD_REQUEST;
	}
	if ( strncasecmp(elem,"GET",strlen("GET"))!=0 ) {
		return HTTP_CLIENT_ERR_BAD_REQUEST;
	}
	
	//extract tx file name
	elem = strtok(NULL," "); //point to next active
	printf ("Second extracted element for http_req:%s\n",elem);
	//if ( (elem=='\0') || (strncasecmp(elem[0],"HTTP",strlen("HTTP"))!=0) {
	if ( elem=='\0') {
		return HTTP_CLIENT_ERR_BAD_REQUEST;
	}
	if (access(elem,F_OK)!=0) { // File is not existed
		return HTTP_CLIENT_ERR_FILE_NOT_FOUND;
	} else if (access(elem,R_OK)!=0) {
		return HTTP_CLIENT_ERR_FORBIDDEN;
	}
	//tx_file_name = elem ;
	sprintf(tx_file_name,elem,strlen(elem));
	printf ("Extracted filename for http_req:%s\n",tx_file_name);
	return HTTP_SUCCESS_200;	
}
/*
void childshandle(int signal);
double client_cmd_function (int rcd){ 
	return sqrt (rcd);
}
*/
