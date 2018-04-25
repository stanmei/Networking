/* File : usercli.c
 *
 * Author : grp1
 * 
 * Desc : this is client user interface functions.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#include <cJSON.h>
#include <authenuser.h>

#include "user_mannual.h"
#include "usercli.h"
#include "connexch.h"

#define MAX_ARG_NUMS 4
#define MAX_ARG_LEN 64

#define MAX_RBUF_SIZE 1024

int UserCli (int client_sock,char* user_grp,char* user_name,char* user_pswd) {
	// "admin" user group operation	
	if (!strcmp(user_grp,"admin")) {
		admin_opers(client_sock,user_grp,user_name,user_pswd);
	}
	else {
		printf ("Unrecogized user group!\n");
		exit(-1);
	} 
	return 0;
}


// admin operations
void admin_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) {


	int valid_op ,ret;
	do 
	{
		//char* in_cmd_main = {0};
		char in_cmd_main[MAX_ARG_NUMS]={0};
		//char in_argus[MAX_ARG_LEN][MAX_ARG_NUMS]= {0};
		char* in_argus[]= {"NULL","NULL","NULL"};

		char in_argus0[MAX_ARG_LEN]= {0};
		char in_argus1[MAX_ARG_LEN]= {0};
		char in_argus2[MAX_ARG_LEN]= {0};


		admin_manual(in_cmd_main);
		valid_op = 1;

		if ( !strcmp(in_cmd_main,"a" ) ) {			
			strcpy(in_cmd_main,"create_accnt");			
			admin_create_accnt_manual(in_argus0,in_argus1,in_argus2);
		} else if (!strcmp(in_cmd_main,"b")) {
			strcpy(in_cmd_main,"del_accnt");
			admin_del_accnt_manual(in_argus1);
		} else if (!strcmp(in_cmd_main,"c")) {
			strcpy(in_cmd_main,"upd_pswd");
			modify_manual(in_argus1,in_argus2);
		} else if (!strcmp(in_cmd_main,"d")) {
			strcpy(in_cmd_main,"show_list");
			qry_manual(in_argus0);
		} else if (!strcmp(in_cmd_main,"e")) {
			strcpy(in_cmd_main,"exit");
			//close(client_sock);
			//exit(0);
		} else {
			printf ("No supported operations\n");
			valid_op = 0;
		}
		in_argus[0]= in_argus0;
		in_argus[1]= in_argus1;
		in_argus[2]= in_argus2;

		if (valid_op) {
			printf ("User %s send commands(%s) to server with argus:%s,%s,%s\n",user_name,in_cmd_main,in_argus[0],in_argus[1],in_argus[2]);
			ConnTxCjsonnt_clt (client_sock,user_grp,user_name,user_pswd,in_cmd_main,in_argus);
			

		        // receive ack  message from server
        		char ret_msgs[MAX_RBUF_SIZE]= {0};
        		ret= ConnRxCjsonnt_clt (client_sock,ret_msgs);

		        if (ret< 0) {
               		 	printf("Failed to receive ack message from server:%s\n",ret_msgs);
       			 }

		}

		if (!strcmp(in_cmd_main,"exit") )
		{
			close(client_sock);
			exit(0);
		}
	} while (1) ;

}

