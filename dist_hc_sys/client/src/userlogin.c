/* File: userlogin.c
 *
 * Author: grp 1
 *
 * Desc: user login with grp, username, pswd
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 

#define MAX_IN_LEN 64

void GetInputs(char* in,int length,int mask);

int UserLogin (char* grp,char* username,char* pswd) {
	
	printf("User Group:");
	GetInputs(grp,MAX_IN_LEN,0);
	
	printf("User Name:");
	GetInputs(username,MAX_IN_LEN,0);
	
	printf("Password:");
	GetInputs(pswd,MAX_IN_LEN,1);
	
	return 0;
}

void GetInputs(char* in,int length,int mask) {
	char inchar ;
	int idx=0;
	
	do 
	{
		inchar=getchar();
		//resume blank
		if (inchar==' ') {
			continue;
		}
		// backspace
		if (inchar=='\b') {
			idx--;
			continue;
		}
		// enter
		//if (inchar=='\r') {
		if (inchar=='\n') {	
			break;
		}
		// max length limitation
		if ( idx>=length-1) {
			continue;
		}
		in[idx] = inchar;
		if (mask==1) {
			printf("*");
		} 
		
		idx++;
	} while (inchar!='\r') ;
	
	in[idx]='\0';
	//printf("\n");
	return ;
	
}
