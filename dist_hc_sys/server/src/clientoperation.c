/* File : clientoperation.c
 *
 * Author : grp1
 *
 * Desc : client operation 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 

#include <sys/socket.h>
#include <sys/types.h> 

#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#include <cJSON.h>
#include <connexch.h>
#include <sqldbop.h>
#include <clientopeartion.h>

#define MAX_USER_LEN 256
#define MAX_ARGS_NUM 2
#define MAX_MSG_ROWS 16

void* ClientOperation(void* argument) {
	
	int accept_sock = *((int*) argument) ;
/*
    	char name[MAX_USER_LEN] = {0};
	char password[MAX_USER_LEN] = {0};
	char grp[MAX_USER_LEN] = {0};

	char cmd[MAX_USER_LEN] = {0};
	char* ary_arguments[] ={"NULL","NULL","NULL"};
	char* qry_rslt_rows[MAX_MSG_ROWS] ;
	int qry_row_num=0;
*/
	/*
	 * SQL 
	 */
    
	int ret;
	while (1) 
	{
	char name[MAX_USER_LEN] = {0};
	char password[MAX_USER_LEN] = {0};
	char grp[MAX_USER_LEN] = {0};

	char cmd[MAX_USER_LEN] = {0};
	char* ary_arguments[] ={"NULL","NULL","NULL"};
	char* qry_rslt_rows[MAX_MSG_ROWS] ={NULL};
	//char qry_rslt_rows[MAX_USER_LEN][MAX_MSG_ROWS]={NULL};
	//qry_rslt_rows [MAX_MSG_ROWS] = (char*) malloc(MAX_MSG_ROWS*MAX_USER_LEN*sizeof(char));
	for (int i =0; i<MAX_MSG_ROWS;i++)
        	qry_rslt_rows[i] = malloc(MAX_USER_LEN*sizeof(char));
	int qry_row_num=0;

	//char* user_grp={0}, *user_name={0},*user_pswd={0},*cmd={0};
	ret= ConnRxCjsonnt_srv (accept_sock,grp,name,password,cmd,ary_arguments);
	if ( ret <0 ) {
		printf("Abnormal message received from client,exit.\n");
		close(accept_sock);
		return NULL;
	} 

	//client operations
	char ret_msg[MAX_USER_LEN];
	//char* ret_msg[];

	if ( !strcmp(cmd,"auth") ) {
		ret= Authenticate(grp,name,password);
	 	//strcpy(ret_msg,"Authen ack!");
		sprintf(ret_msg,"Authen ack!");
		printf("authenticat return to client:%d; ret_msg:%s\n",ret,ret_msg);
	} else if (!strcmp(cmd,"create_accnt")) {
		char* new_grp = ary_arguments[0];
		char* new_name= ary_arguments[1];
		char* new_passwd= ary_arguments[2];

		ret= Authenticate(grp,name,password);
		if (ret<0 )  
	 		sprintf(ret_msg,"create_accnt %s fail: request from non existed user (grp:%s,name:%s),code:%d !",new_name,grp,name,ret);
		else 
		{
			ret= Create_Accnt(new_grp,new_name,new_passwd);
			if ( ret < 0 ) 
	 			sprintf(ret_msg,"create_accnt %s fail to create with code %d!",new_name,ret);
			else 
	 			sprintf(ret_msg,"create_accnt %s successfully ack!",new_name);
		}

	} else if (!strcmp(cmd,"del_accnt")) {
		char* del_name= ary_arguments[1];
		ret= Authenticate(grp,name,password);

		if ( ret>=0 )  
			ret= Delete_Accnt(del_name);

		if ( ret == -1 ) 
	 		sprintf(ret_msg,"delete_accnt %s fail due non-exist object!",del_name);
		else if (ret<0)
	 		sprintf(ret_msg,"delete_accnt %s fail!",del_name);
		else 
	 		sprintf(ret_msg,"delete_accnt %s successfully ack!",cmd);

	} else if (!strcmp(cmd,"upd_pswd")) {
		char* upd_name= ary_arguments[1];
		char* upd_val= ary_arguments[2];
		ret= Authenticate(grp,name,password);

		if ( ret>=0 )  
			ret= Update_Item(upd_name,upd_val);

		if ( ret == -1 ) 
	 		sprintf(ret_msg,"update_pswd <%s> fail due non-exist object!",upd_name);
		else if (ret<0)
	 		sprintf(ret_msg,"update_pswd <%s> fail!",upd_name);
		else 
	 		sprintf(ret_msg,"update_pswd <%s> successfully ack!",cmd);

	} else if (!strcmp(cmd,"show_list")) {
		char* qry_tbl= ary_arguments[0];		
		
		ret=Query_Tbl (qry_tbl,&qry_row_num,qry_rslt_rows);
		sprintf(ret_msg,"show list ack!"); 
		/*
		//printf ("total qry ret rows:%d :\n",qry_row_num);
		//for (int i=0; i<qry_row_num;i++) 
			printf ("%s \n",qry_rslt_rows[0]);
			printf ("%s \n",qry_rslt_rows[1]);
		*/
		printf("\n");
	} else if (!strcmp(cmd,"exit")) {
		ret = 0 ;
	 	//strcpy(ret_msg,"exit!");
		sprintf(ret_msg,"exit!");
		ConnTxCjsonnt_srv (accept_sock,ret,ret_msg,NULL,0);
		return NULL;	
	} else {
		printf("Abnoral commands '%s' be received from clients.\n",cmd);
		sprintf(ret_msg,"Abnormal commands '%s'.\n",cmd);
		//exit(-1);
	}
	
	/*
	 * Send result back clients.
	 */
	ret=ConnTxCjsonnt_srv (accept_sock,ret,ret_msg,qry_rslt_rows,qry_row_num);
	if (ret <0) {
		printf("Abnormal message transmit to client,exit.\n");
		close(accept_sock);
		exit(-1);
	} 
	/*
	else {
		printf("[clientopers]Send ack message to client from sock %d,ret_val:%d,ret_msg:[%s].\n",accept_sock,ret,ret_msg);
	}
	*/
	//??? wireshark:this will triger fin-ack to client.
	//close(accept_sock);
	}

	return NULL;	
}
