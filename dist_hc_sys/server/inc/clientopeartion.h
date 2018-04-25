#ifndef _CLIENTOPERATION_H
#define  CLIENTOPERATION_H

void* ClientOperation(void* argument) ;

//cJSON* ConnRx (int socket);

int ConnRxCjsonnt_srv (int accept_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]) ;

int ConnTxCjsonnt_srv (int accept_sock,int ret_val, char* ret_msg, char* ary_ret_msg[],int ary_ret_len) ;

#endif
