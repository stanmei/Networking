/* File : usercli.c
 *
 * Author : grp1
 * 
 * Desc : this is client user interface functions.
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
#include <string.h>
/***********End of Winsock**************/
#endif

#include <cJSON.h>
#include <authenuser.h>

#include "user_mannual.h"
#include "usercli.h"
#include "connexch.h"

#define MAX_ARG_NUMS 4
#define MAX_ARG_LEN 64//128

#define MAX_RBUF_SIZE 128//1024

int UserCli (int client_sock,char* user_grp,char* user_name,char* user_pswd) {
	// "admin" user group operation	
	if (!strcmp(user_grp,"admin")) {
		admin_opers(client_sock,user_grp,user_name,user_pswd);
	}
	// "healthcare" user group operation
	else if (!strcmp(user_grp,"healthcare")) {
		healthcare_opers(client_sock,user_grp,user_name,user_pswd);
	}
	// "insurance" user group operation
	else if (!strcmp(user_grp,"insurance")) {
		insurance_opers(client_sock,user_grp,user_name,user_pswd);
	}	
	else {
		printf ("Unrecogized user group!\n");
		exit(-1);
	} 
	return 0;
}

/*
 * admin operations
 */
void admin_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) {


	int valid_op ,ret;
	do 
	{
		//char* in_cmd_main = {0};
		//char in_cmd_main[MAX_ARG_NUMS]={0}; // wins : incmd_main,valid_op be modifed.maybe caused by overcover...
		char in_cmd_main[MAX_ARG_LEN]={0};
		//char in_argus[MAX_ARG_LEN][MAX_ARG_NUMS]= {0};
		char* in_argus[]= {"NULL","NULL","NULL"};

		char in_argus0[MAX_ARG_LEN]= {0};
		char in_argus1[MAX_ARG_LEN]= {0};
		char in_argus2[MAX_ARG_LEN]= {0};
		char in_argus3[MAX_RBUF_SIZE]= {0};

		admin_manual(in_cmd_main);
		valid_op = 1;

		if ( !strcmp(in_cmd_main,"a" ) ) {			
			strcpy(in_cmd_main,"create_accnt");
			admin_create_accnt_manual(in_argus0,in_argus1,in_argus2);
			printf("new in_cmd_main(a):%s,valid_op:%d\n",in_cmd_main,valid_op);
			valid_op = 1;
		} else if (!strcmp(in_cmd_main,"b")) {
			strcpy(in_cmd_main,"del_accnt");
			admin_del_accnt_manual(in_argus1);
			valid_op = 1;
			printf("new in_cmd_main(b):%s\n",in_cmd_main);
		} else if (!strcmp(in_cmd_main,"c")) {
			strcpy(in_cmd_main,"upd_pswd");
			strcpy(in_argus0,"users");
			strcpy(in_argus2,"password");
			modify_manual(in_argus1,in_argus3);
			valid_op = 1;
			printf("new in_cmd_main(c):%s\n",in_cmd_main);
		} else if (!strcmp(in_cmd_main,"d")) {
			strcpy(in_cmd_main,"show_list");
			qry_manual(in_argus0); //tbl=input;
			strcpy(in_argus1 ,"*") ; //item=any;
			valid_op = 1;
			printf("new in_cmd_main(d):%s\n",in_cmd_main);
		} else if (!strcmp(in_cmd_main,"e")) {
			strcpy(in_cmd_main,"exit");
			valid_op = 1;
			//close(client_sock);
			//exit(0);
			printf("new in_cmd_main(e):%s\n",in_cmd_main);
		} else {
			printf ("No supported operations\n");
			valid_op = 0;
		}
		printf("new in_cmd_main(-):%s,valid_op:%d \n",in_cmd_main,valid_op);
		in_argus[0]= in_argus0;
		in_argus[1]= in_argus1;
		in_argus[2]= in_argus2;
		in_argus[3]= in_argus3;

		printf ("User %s input commands '%s' to server, valid op:%d, with argus:%s,%s,%s,%s.\n",user_name,in_cmd_main,valid_op,in_argus[0],in_argus[1],in_argus[2],in_argus[3]);

		if (valid_op) {
			printf ("User %s prepare to send commands(%s) to server with argus:%s,%s,%s,%s.\n",user_name,in_cmd_main,in_argus[0],in_argus[1],in_argus[2],in_argus[3]);
			int ary_argus_len = 4;
			ConnTxCjsonnt_clt (client_sock,user_grp,user_name,user_pswd,in_cmd_main,in_argus,ary_argus_len);
			
			printf ("User %s sent commands(%s) to server with argus:%s,%s,%s,%s.\n",user_name,in_cmd_main,in_argus[0],in_argus[1],in_argus[2],in_argus[3]);
		        // receive ack  message from server
        		char ret_msgs[MAX_RBUF_SIZE]= {0};
        		ret= ConnRxCjsonnt_clt (client_sock,ret_msgs);

		        if (ret< 0) {
               		 	printf("Failed to receive ack message from server:%s\n",ret_msgs);
       			 }

		}

		if (!strcmp(in_cmd_main,"exit") )
		{
#ifdef __LINUX
			close(client_sock);
#endif
#ifdef __WINDOWS
			closesocket(client_sock);
			WSACleanup();
#endif
			exit(0);
		}
	} while (1) ;

}

/*
 * healthcare operations
 */
void healthcare_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) {


	int valid_op ,ret;
	do 
	{
		//char* in_cmd_main = {0};
		//char in_cmd_main[MAX_ARG_NUMS]={0}; // wins : incmd_main,valid_op be modifed.maybe caused by overcover...
		char in_cmd_main[MAX_ARG_LEN]={0};
		//char in_argus[MAX_ARG_LEN][MAX_ARG_NUMS]= {0};
		char* in_argus[]= {"NULL","NULL","NULL"};

		char in_argus0[MAX_ARG_LEN]= {0};
		char in_argus1[MAX_ARG_LEN]= {0};
		char in_argus2[MAX_ARG_LEN]= {0};
		char in_argus3[MAX_RBUF_SIZE]= {0};

		healthcare_manual(in_cmd_main);
		valid_op = 1;

		if (!strcmp(in_cmd_main,"a")) {
			strcpy(in_cmd_main,"upd_pswd");
			strcpy(in_argus0,"users");
			strcpy(in_argus2,"password");
			modify_manual(in_argus1,in_argus3);
		} else if ( !strcmp(in_cmd_main,"b" ) ) {			
			strcpy(in_cmd_main,"create_patient");			
			healthcare_create_accnt_manual(in_argus1,in_argus2,in_argus3); //input: 2-patient_name,3-patient record.
		} else if (!strcmp(in_cmd_main,"c")) {
			strcpy(in_cmd_main,"show_list");
			healthcare_qry_manual(in_argus1); //item=input;
			strcpy(in_argus0 ,"patients") ; //tbl=patients;
		} else if (!strcmp(in_cmd_main,"d")) {
			strcpy(in_cmd_main,"upd_patient");
			strcpy(in_argus0,"patients"); // 0-tbl name
			strcpy(in_argus2,"record"); // 2-new item name
			healthcare_modify_manual(in_argus1,in_argus3);// 1-patient 2-name; 3- new record
			
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
		in_argus[3]= in_argus3;

		if (valid_op) {
			//printf ("User %s send commands(%s) to server with argus:%s,%s,%s,%s.\n",user_name,in_cmd_main,in_argus[0],in_argus[1],in_argus[2],in_argus[3]);
			int ary_argus_len = 4;
			ConnTxCjsonnt_clt (client_sock,user_grp,user_name,user_pswd,in_cmd_main,in_argus,ary_argus_len);
			

		        // receive ack  message from server
        		char ret_msgs[MAX_RBUF_SIZE]= {0};
        		ret= ConnRxCjsonnt_clt (client_sock,ret_msgs);

		        if (ret< 0) {
               		 	printf("Failed to receive ack message from server:%s\n",ret_msgs);
       			 }

		}

		if (!strcmp(in_cmd_main,"exit") )
		{
#ifdef __LINUX
			close(client_sock);
#endif
#ifdef __WINDOWS
			closesocket(client_sock);
			WSACleanup();
#endif
			exit(0);
		}
	} while (1) ;

}


/*
 * insurance operations
 */
void insurance_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) {


	int valid_op ,ret;
	do 
	{
		//char* in_cmd_main = {0};
		//char in_cmd_main[MAX_ARG_NUMS]={0}; // wins : incmd_main,valid_op be modifed.maybe caused by overcover...
		char in_cmd_main[MAX_ARG_LEN]={0};
		//char in_argus[MAX_ARG_LEN][MAX_ARG_NUMS]= {0};
		char* in_argus[]= {"NULL","NULL","NULL"};

		char in_argus0[MAX_ARG_LEN]= {0};
		char in_argus1[MAX_ARG_LEN]= {0};
		char in_argus2[MAX_ARG_LEN]= {0};
		char in_argus3[MAX_RBUF_SIZE]= {0};

		insurance_manual(in_cmd_main);
		valid_op = 1;

		if (!strcmp(in_cmd_main,"a")) {
			strcpy(in_cmd_main,"upd_pswd");
			strcpy(in_argus0,"users");
			strcpy(in_argus2,"password");
			modify_manual(in_argus1,in_argus3);		
		} else if (!strcmp(in_cmd_main,"b")) {
			strcpy(in_cmd_main,"show_list");
			healthcare_qry_manual(in_argus1); //item=input;
			strcpy(in_argus0 ,"patients") ; //tbl=patients;
		} else if (!strcmp(in_cmd_main,"c")) {
			strcpy(in_cmd_main,"upd_patient");
			strcpy(in_argus0,"patients"); // 0-tbl name
			strcpy(in_argus2,"insurance_coverable"); // 2-new item name
			healthcare_modify_manual(in_argus1,in_argus3);// 1-patient 2-name; 3- new record
			
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
		in_argus[3]= in_argus3;

		if (valid_op) {
			//printf ("User %s send commands(%s) to server with argus:%s,%s,%s,%s.\n",user_name,in_cmd_main,in_argus[0],in_argus[1],in_argus[2],in_argus[3]);
			int ary_argus_len = 4;
			ConnTxCjsonnt_clt (client_sock,user_grp,user_name,user_pswd,in_cmd_main,in_argus,ary_argus_len);
			

		        // receive ack  message from server
        		char ret_msgs[MAX_RBUF_SIZE]= {0};
        		ret= ConnRxCjsonnt_clt (client_sock,ret_msgs);

		        if (ret< 0) {
               		 	printf("Failed to receive ack message from server:%s\n",ret_msgs);
       			 }

		}

		if (!strcmp(in_cmd_main,"exit") )
		{
#ifdef __LINUX
			close(client_sock);
#endif
#ifdef __WINDOWS
			closesocket(client_sock);
			WSACleanup();
#endif
			exit(0);
		}
	} while (1) ;

}


