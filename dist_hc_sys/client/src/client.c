/* File name: client.c
 *
 * Author: grp1
 * Desc: This is client main function for distribute healthcare system.It provides client 
 * commands input and execution for: 
 * admin, healthcare provider,insurance,patients.
 *
 * Interaction without gui:
 * ./client <server ip> <port>
 * user group: <admin/healthcare/insurance/paitents> 
 * user name:  <user name>
 * password:   <user password>
 * 
 * commands: <user commands>
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

#include <cJSON.h>

#include <connserver.h>
#include <userlogin.h>
#include <authenuser.h>
#include <usercli.h>

#define MAX_GRP_LEN 64
#define MAX_USER_NAME_LEN 64
#define MAX_PSWD_LEN 64

#define MAX_RETRY 4


int main (int argc, char* argv[]) {
	
	if ( argc != 3  ) {
		//ClientHelp();
		printf ("Abnoraml arguments!\n");
		exit(-1);
	}
	
	struct hostent* server_ip = gethostbyname(argv [1]);
	int server_port = atoi(argv[2]);	

	// Connect with server
	int client_sock ;
	int ret = 0;
	client_sock = ConnServer(server_ip,server_port) ;
	if ( client_sock< 0 ) {
		printf ("Fail to connect with server!\n");
		exit(-1);
	} else {
		printf ("Connected with server : %s,port:%d through client socket %d........ \n",server_ip->h_name,server_port,client_sock);
	}
		
	// user inter-action
	char user_grp [MAX_GRP_LEN] = {0};
	char user_name [MAX_USER_NAME_LEN] = {0};
	char user_pswd [MAX_PSWD_LEN] = {0};

	// return value decleration
	int retry_cnt=0;
	
	// User authentication
	//while (retry_cnt<MAX_RETRY) {
	do 
	{
		ret = UserLogin(user_grp,user_name,user_pswd);
		if (ret < 0) {
			printf ("Abnormal User Inputs, Exit!\n");
			exit(-1);
		}
		
		printf ("\n");
		
		printf ("Authening with server........\n");
		// authentication with server
		ret = AuthenUser(client_sock,user_grp,user_name,user_pswd);
		if ( ret<0) {
			//printf ("Fail to authentication, please double check user information!\n");
			printf ("please double check and retry!\n");
			if ( retry_cnt== (MAX_RETRY-1)) {
				close(client_sock);
				exit(-1);
			}
		} else {
			printf ("Authen success(grp:%s,user name: %s)\n",user_grp,user_name);
			printf("----------------------------\n");
			printf("\n");
			break;

		} 
		printf("----------------------------\n");
		retry_cnt++;
	} while (retry_cnt<MAX_RETRY) ;
			
	// user commands
	ret = UserCli (client_sock,user_grp,user_name,user_pswd);
	printf ("Thank you. Bye!\n");
	return 0;	
}


