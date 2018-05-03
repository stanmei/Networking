/* File: userlogin.c
 *
 * Author: grp 1
 *
 * Desc: user login with grp, username, pswd
 */
#define __WINDOWS
//#define __LINUX

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

//#ifdef __LLINUX
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
/*
#endif

#ifdef __WINDOWS
void GetInputs(char* in,int length,int mask) {
    //char buffer[81];
    int i, ch;

    for (i = 0; (i < length) && ((ch = getchar()) != EOF)
                         && (ch != '\n'); i++)
    {
        in[i] = (char) ch;
    }

    // Terminate string with a null character
    in[i] = '\0';
    sprintf( "Input was: %s\n", in);
}
#endif
*/
