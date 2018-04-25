
int ConnTx(int socket,cJSON* txmsg);

cJSON* ConnRx(int socket);

int ConnTxCjsonnt_clt (int client_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]);

int ConnRxCjsonnt_clt (int accept_sock,char* ret_msgs);

int ConnRxCjsonnt_srv (int accept_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]) ;

int ConnTxCjsonnt_srv (int accept_sock,int ret_val, char* ret_msg, char* ary_ret_msg[],int ary_ret_len) ;
