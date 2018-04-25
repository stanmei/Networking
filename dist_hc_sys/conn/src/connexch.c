/* File: conntx.c
 *
 * Author: grp1
 * 
 * Desc : this is tx cjson format message via tcp connection.
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

#define MAX_RBUF_SIZE 1024

int ConnTx (int socket,cJSON* txmsg) {
	
	char* json_str = NULL;
	char* txbuf = NULL ;
	
	int ret = 0;
	// Allocate memory for tx cjson message.
	json_str = cJSON_Print (txmsg);
	txbuf = malloc(strlen(json_str)+1) ;
	memset (txbuf,0,strlen(json_str));
	memcpy (txbuf,json_str,strlen(json_str)) ;
	
	int txlen = write(socket,txbuf,strlen(json_str)) ;
	if (txlen != strlen(json_str)) {
		printf ("Abnormal tx message for socket: %d. \n",socket);
		ret = -1;
	}
	//debugging 
	//else {
		//printf ("[ConnTx]: send message from socket %d to remote : %s.\n", socket,txbuf);
	//}
	
	
	//cJSON_Delete (txmsg);
	cJSON_free(json_str);
	free(txbuf);
	
	return ret;
}

cJSON* ConnRx (int socket) {
	
	char rxbuf [MAX_RBUF_SIZE] = {0};
	
	// read from socket
	int rxlen = read(socket,rxbuf,MAX_RBUF_SIZE);
	
	if ( rxlen < 0 ) {
		printf ("Failed to read, error code : %s \n",strerror(errno));
		return NULL ;
	}
	/*
	//debugging 
	else {
		printf ("[ConnRx]: send message from socket %d to remote : %s.\n", socket,rxbuf);
	}
	*/
	 
	// Return cjson format message
	return cJSON_Parse(rxbuf);
}

/* 
 *client send and receive function
 */

//int ConnTxCjsonnt_clt (int client_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]) {
int ConnTxCjsonnt_clt (int client_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[],int ary_len) {
	cJSON* cjson_txmsg = NULL ;
	//printf("[conn] Arguments to be sent from client to server: %s, %s\n",ary_arguments[0],ary_arguments[1]);
	
        // Create cjson txmsg to server
        cjson_txmsg = cJSON_CreateObject();

        cJSON_AddItemToObject (cjson_txmsg,"grp",cJSON_CreateString(user_grp));
        cJSON_AddItemToObject (cjson_txmsg,"name",cJSON_CreateString(user_name));
        cJSON_AddItemToObject (cjson_txmsg,"passwd",cJSON_CreateString(user_pswd));
        cJSON_AddItemToObject (cjson_txmsg,"cmd",cJSON_CreateString(cmd));
        //cJSON_AddItemToObject (cjson_txmsg,"ary_arguments",cJSON_CreateString(ary_arguments));
	cJSON* in_args = cJSON_CreateArray() ;
	for (int i=0;i<ary_len;i++) {
		cJSON_AddItemToArray(in_args,cJSON_CreateString(ary_arguments[i]));
	}
	/*
	cJSON_AddItemToArray(in_args,cJSON_CreateString(ary_arguments[0]));
	cJSON_AddItemToArray(in_args,cJSON_CreateString(ary_arguments[1]));
	cJSON_AddItemToArray(in_args,cJSON_CreateString(ary_arguments[2]));
	*/
        cJSON_AddItemToObject (cjson_txmsg,"arguments",in_args);

	//debugging-print whole tx cjson message.
        char* str_cjson ;
        str_cjson = cJSON_Print(cjson_txmsg) ;
        printf ("Sending auth message with cjson :%s \n", str_cjson);

        // send authen message to server
        int ret = ConnTx(client_sock, cjson_txmsg);
        if (ret<0) {
                printf ("Failed to send cjson message to server\n");
		cJSON_Delete (cjson_txmsg);
                exit(-1);
        }

	cJSON_Delete (cjson_txmsg);

	return ret;
}

// Client rx cjson message from server
int ConnRxCjsonnt_clt (int accept_sock,char* ret_msgs) {

	cJSON* rxmsg_cjson = NULL ;
        rxmsg_cjson = ConnRx (accept_sock);

        // debug printing
        char* str_rxmsg_cjson ;
        str_rxmsg_cjson = cJSON_Print(rxmsg_cjson) ;
        printf ("Received cjson message: %s \n",str_rxmsg_cjson);

	// extract fields from cjson
        cJSON* ret_val_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "return");
        cJSON* ary_ret_msgs_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "ret_msgs");
	cJSON* ret_msgs_elem_obj;

	int ary_ret_msgs_len = cJSON_GetArraySize(ary_ret_msgs_json) ;

	if (ary_ret_msgs_len>0){
		for(int idx=0;idx<ary_ret_msgs_len;idx++){
			ret_msgs_elem_obj = cJSON_GetArrayItem(ary_ret_msgs_json,idx);
			printf("%s",cJSON_GetStringValue(ret_msgs_elem_obj));
		}
	}

	printf("received msg len from server:%d, ret value:;\n",ary_ret_msgs_len); 
	//printf("received msg len from server:%d, ret value:%d;\n",ary_ret_msgs_len,(int)ret_val_json ->valuedouble); 
	return (int)ret_val_json ->valuedouble;
}

	/* 
	 * server send and receive function
	 */

int ConnRxCjsonnt_srv (int accept_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]) {

	cJSON* rxmsg_cjson = NULL ;
	rxmsg_cjson = ConnRx (accept_sock);

	//printf("[conn] Arguments to be received from client to server: %s, %s.\n",ary_arguments[0],ary_arguments[1]);

	// debugging
	char* str_rxmsg_cjson ;
	str_rxmsg_cjson = cJSON_Print(rxmsg_cjson) ;
	printf ("Received cjson message: %s \n",str_rxmsg_cjson);

	// extract fields from cjson message
	cJSON* grp_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "grp");
	cJSON* name_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "name");
	cJSON* password_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "passwd");
	cJSON* cmd_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "cmd");

	cJSON* ary_args_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "arguments");
	//cJSON* args_elem_obj;

	// extract cjson object into chars
	int ary_args_len = cJSON_GetArraySize(ary_args_json) ;

        //printf("recived args lengths:%d;\n",ary_args_len);
	if (ary_args_len>0) {
		for(int idx=0;idx<ary_args_len;idx++){
			cJSON* args_elem_obj = cJSON_GetArrayItem(ary_args_json,idx);
			//strncpy(ary_arguments[idx],cJSON_GetStringValue(args_elem_obj),strlen(cJSON_GetStringValue(args_elem_obj))); -> wrong, 
			ary_arguments[idx]=cJSON_GetStringValue(args_elem_obj);
			//printf ("args array unit %d :%s.\n",idx,ary_arguments[idx] );
		}
	}

	if(cJSON_IsString(grp_json)) {
		 strncpy(user_grp, (char *) grp_json->valuestring, strlen((char *) grp_json->valuestring));
	 //printf("Received user_grp:%s,user_name:%s,user_pswd:%s;\n",user_grp,user_name,user_pswd);
	} else {
		 printf(" Invalid user group format from cJSON message \n");
		 exit(-1);
	 }
	    
	 if(cJSON_IsString(name_json)) {
		strncpy(user_name, (char *) name_json->valuestring, strlen((char *) name_json->valuestring));
	} else {
		printf(" Invalid user name format from cJSON message \n");
		exit(-1);
	}
	    
	if(cJSON_IsString(password_json)) {
		strncpy(user_pswd, (char *) password_json->valuestring, strlen((char *) password_json->valuestring));
		printf("Received passwd:%s.\n",user_pswd);
	} else {
		printf(" Invalid user password format from cJSON message \n");
		exit(-1);
	}

	if(cJSON_IsString(cmd_json)) {
		strncpy(cmd, (char *) cmd_json->valuestring, strlen((char *) cmd_json->valuestring));
		//strcpy(cmd, (char *) cmd_json->valuestring);
       		printf("Received cmd:%s (%s).\n",cmd,(char *) cmd_json->valuestring);
	} else {
        	printf(" Invalid user cmd format from cJSON message \n");
        	exit(-1);
    	}

	return 0;
}

int ConnTxCjsonnt_srv (int accept_sock,int ret_val, char* ret_msg, char* ary_ret_msg[],int ary_ret_len) {

	cJSON* ret_msg_json = cJSON_CreateObject ();

        //char* ret_msg = "authenticaiton";
        cJSON_AddNumberToObject(ret_msg_json,"return",ret_val);
        cJSON_AddItemToObject(ret_msg_json,"message",cJSON_CreateString(ret_msg));

	//arrary
	cJSON* qry_rows = cJSON_CreateArray() ;
	for(int i=0;i<ary_ret_len;i++) {
		cJSON_AddItemToArray(qry_rows,cJSON_CreateString(ary_ret_msg[i]));
		//printf("[ConnTxCjsonnt_srv] ary_ret_msg idx: %d, msg: %s.\n",i,ary_ret_msg[i]);
	}
        cJSON_AddItemToObject (ret_msg_json,"ret_arrary",qry_rows);

        int tx_num = ConnTx (accept_sock,ret_msg_json);

	// debugging
	char* str_txmsg_cjson ;
	str_txmsg_cjson = cJSON_Print(ret_msg_json) ;
	printf ("Transmit cjson message: %s \n",str_txmsg_cjson);

	cJSON_Delete (ret_msg_json);

        if (tx_num <0 ) exit(-1);

	return 0;
}


