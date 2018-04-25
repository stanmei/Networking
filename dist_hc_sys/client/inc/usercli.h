#ifndef _USERCLI_H
#define _USERCLI_H

int UserCli (int client_sock,char* user_grp,char* user_name,char* user_pswd);

void admin_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd);
void healthcare_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) ;
void insurance_opers(int client_sock,char* user_grp,char* user_name,char* user_pswd) ;

#endif
