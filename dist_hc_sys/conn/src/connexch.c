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
	
	cJSON_Delete (txmsg);
	//cJson_Free(json_str);
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
	 
	// Return cjson format message
	return cJSON_Parse(rxbuf);
}