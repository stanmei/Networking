
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_mannual.h"

#define MAX_USER_NAME_LEN 64
#define MAX_PSWD_LEN 64

extern void GetInputs(char* in,int length,int mask);

void admin_manual(char* in){
	printf("Please select:\n");
	printf("a : create account;\n");
	printf("b : delete account;\n");
	printf("c : change password;\n");
	printf("d : show account list;\n");
	printf("e : exit;\n");
	//received input option
	int rtry_cnt = 0;

	do 
	{
		printf(">");
		GetInputs(in,2,0); 
		//printf("input option:%s.\n",in);
		if ( !strcmp(in,"a") ||
		     !strcmp(in,"b") ||
		     !strcmp(in,"c") ||
		     !strcmp(in,"d") ||
		     !strcmp(in,"e") ) {
		     //printf("valid option:%s.\n",in);
		     break;
		}
		else {
		     rtry_cnt++;
		     printf("Not valid option,please re-input.\n");
		}

	} while (rtry_cnt<6);

	if ( rtry_cnt>=6) {
		printf("Too many retry times,exit.\n");
		exit(-1);
	}
      //printf("valid option,exit manual.\n");
}

void admin_create_accnt_manual(char* new_usr_grp,char* new_usr_name,char* new_pswd){
	printf("Please input new account group: ");
	GetInputs(new_usr_grp,MAX_USER_NAME_LEN,0); 
	printf("Please input new account name: ");
	GetInputs(new_usr_name,MAX_USER_NAME_LEN,0); 
	printf("Please input new passowrd: ");
	GetInputs(new_pswd,MAX_PSWD_LEN,0); 
}

void admin_del_accnt_manual(char* del_usr_name){
	printf("Please input account name to be deleted: ");
	GetInputs(del_usr_name,MAX_USER_NAME_LEN,0); 
}

/*
 * clients: doctor / insurance
 * Function: query account
 */
void modify_manual(char* item_name,char* new_item){
	printf("Please input item name to be modified : ");
	GetInputs(item_name,MAX_USER_NAME_LEN,0); 
	printf("Please input new item content: ");
	GetInputs(new_item,MAX_USER_NAME_LEN,0); 
}
/*
 * clients: all
 * Function: query account
 */
void qry_manual(char* qry_usr_name){
	printf("Please input query user name : ");
	GetInputs(qry_usr_name,MAX_USER_NAME_LEN,0); 
}

/*
 * all clients common function
 */
void chg_pswd_manual(char* chg_usr_pswd){
	printf("Please input new password : ");
	GetInputs(chg_usr_pswd,MAX_PSWD_LEN,0); 
}
