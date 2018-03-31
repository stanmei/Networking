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

void* ClientOperation(void* argument) {
	
	int accept_sock = *((int*) argument) ;
	
	cJSON* rxmsg_cjson = NULL ;	
	rxmsg_cjson = ConnRx (accept_sock);
	
	// debugging
	char* str_rxmsg_cjson ;
	str_rxmsg_cjson = cJSON_Print(rxmsg_cjson) ;
	printf ("Received cjson message: %s \n",str_rxmsg_cjson);
	
	/*
	 * SQL 
	 */
	cJSON* grp_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "grp");
	cJSON* name_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "name");
    cJSON* password_json = cJSON_GetObjectItemCaseSensitive(rxmsg_cjson, "password");

    char name[MAX_USER_LEN] = {0};
    char password[MAX_USER_LEN] = {0};
    char grp[MAX_USER_LEN] = {0};

    if(cJSON_IsString(grp_json)) {
        strncpy(grp, (char *) grp_json->valuestring, strlen((char *) grp_json->valuestring));
    } else {
        printf(" Invalid user group format from cJSON message \n");
        exit(-1);
    }
    
    if(cJSON_IsString(name_json)) {
        strncpy(name, (char *) name_json->valuestring, strlen((char *) name_json->valuestring));
    } else {
        printf(" Invalid user name format from cJSON message \n");
        exit(-1);
    }
    
	if(cJSON_IsString(password_json)) {
        strncpy(password, (char *) password_json->valuestring, strlen((char *) password_json->valuestring));
    } else {
        printf(" Invalid user password format from cJSON message \n");
        exit(-1);
    }
    
	int ret= Authenticate(grp,name, password);
	
	/*
	 * Send result back clients.
	 */
	cJSON* ret_msg_json = cJSON_CreateObject ();
	
	char* ret_msg = "authenticaiton";
	cJSON_AddNumberToObject(ret_msg_json,"return",ret);
	cJSON_AddItemToObject(ret_msg_json,"message",cJSON_CreateString(ret_msg));
	
	int tx_num = ConnTx (accept_sock,ret_msg_json);
	
	if (tx_num <0 ) exit(-1);
	
	return NULL;	
}
