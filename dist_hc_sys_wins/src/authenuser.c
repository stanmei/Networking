/* File : authenuser.c
 *
 * Author : grp1
 * 
 * Desc : this is client user authtication request to server with cjson format.
 *
 */
#define __WINDOWS
//#define __LINUX

#ifdef __LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#endif

#ifdef __WINDOWS
#define WIN32_LEAN_AND_MEAN
/***********Winsock**********************/
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#endif
/***********End of Winsock**************/
#include <cJSON.h>
#include <authenuser.h>

#define MAX_ARG_LEN 64
#define MAX_RBUF_SIZE 1024

int AuthenUser(int client_sock,char* user_grp,char* user_name,char* user_pswd) {

	int ret ;

	// send authentication message to client
	char* cmd="auth";
	char* in_args[]={"NULL","NULL","NULL","NULL"};

	printf("Send authentication message to server,grp(%s),user (%s),pswd(%s),cmd(%s),in args: %s,%s.\n",user_grp,user_name,user_pswd,cmd,in_args[0],in_args[1]);
	ret = ConnTxCjsonnt_clt (client_sock,user_grp,user_name,user_pswd,cmd,in_args,4);
	printf("Waiting for authentication ack message from server.........\n");
	if (ret<0) {
		printf("Abnormal send to client!\n");
		return -1;
	}

	// receive authetication message from client
	//int ret_val=0;
	char ret_msgs[MAX_RBUF_SIZE]= {0};
 	ret= ConnRxCjsonnt_clt (client_sock,ret_msgs);

	if (ret< 0) {
		printf("Failed authentication with message code:%s",ret_msgs);
		return -1;
	} 

	return 0;
}
