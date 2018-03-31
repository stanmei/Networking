/* File : authenuser.c
 *
 * Author : grp1
 * 
 * Desc : this is client user authtication request to server with cjson format.
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

#define MAX_RBUF_SIZE 1024

int AuthenUser(int client_sock,char* user_grp,char* user_name,char* user_pswd) {
	cJSON* cjson_txmsg = NULL ;
	cJSON* cjson_rxmsg = NULL ;
	
	// Create cjson txmsg to server
	cjson_txmsg = cJSON_CreateObject();
	
	cJSON_AddItemToObject (cjson_txmsg,"grp",cJSON_CreateString(user_grp));
	cJSON_AddItemToObject (cjson_txmsg,"name",cJSON_CreateString(user_name));
	cJSON_AddItemToObject (cjson_txmsg,"passwd",cJSON_CreateString(user_pswd));	
	
	char* str_cjson ;
	str_cjson = cJSON_Print(cjson_txmsg) ;
	printf ("Sending auth message with cjson :%s \n", str_cjson);
	
	// send authen message to server
	int ret = ConnTx(client_sock, cjson_txmsg);
	if (ret<0) {
		printf ("Failed to send cjson message to server\n");
		exit(-1);
	}
	
	// receive authen return message from server
	cjson_rxmsg = ConnRx (client_sock);
	
	cJSON* ret_json = cJSON_GetObjectItemCaseSensitive(cjson_rxmsg, "return");
	//cJSON* msg_json = cJSON_GetObjectItemCaseSensitive(cjson_rxmsg, "message");

/*
    char ret_val [MAX_RBUF_SIZE] = {0};
    char ret_msg[MAX_RBUF_SIZE] = {0};
    
    if(cJSON_IsString(return)) {
        strncpy(ret_val, (char *) ret_json->valuestring, strlen((char *) ret_json->valuestring));
    } else {
        printf(" Invalid return value format from cJSON message \n");
        return;
    }
    if(cJSON_IsString(message)) {
        strncpy(ret_msg, (char *) msg_json->valuestring, strlen((char *) msg_json->valuestring));
    } else {
        printf(" Invalid return message format from cJSON message \n");
        return;
    }    
    
    return ret_val;
*/
	return (int) ret_json->valuedouble ;
}