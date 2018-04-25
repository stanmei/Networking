#ifndef _AUTHENUSER_H
#define _AUTHENUSER_H

int AuthenUser(int client_sock,char* user_grp,char* user_name,char* user_pswd);

//int ConnTx(int sock, cJSON* msg_cjson);
//cJSON* ConnRx(int sock);

int ConnTxCjsonnt_clt (int client_sock,char* user_grp,char* user_name,char* user_pswd,char* cmd,char* ary_arguments[]);
int ConnRxCjsonnt_clt (int accept_sock,char* ret_msgs);

#endif
