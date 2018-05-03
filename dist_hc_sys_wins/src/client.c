/* File name: client.c
 *
 * Author: grp1
 * Desc: This is client main function for distribute healthcare system.It provides client 
 * commands input and execution for: 
 * admin, healthcare provider,insurance,patients.
 *
 * Interaction without gui:
 * ./client <server ip> <port>
 * user group: <admin/healthcare/insurance/paitents> 
 * user name:  <user name>
 * password:   <user password>
 * 
 * commands: <user commands>
 *
 * Notes:
 * This is immigration from unix to windows. Reference microsoft example:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/bb530741(v=vs.85).aspx
 */
#define __WINDOWS
//#define __LINUX

//Linux specfic
#ifdef __LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 

#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/wait.h> 

#include <netdb.h>
#include <netinet/in.h>
#endif

#ifdef __WINDOWS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <cJSON.h>

//#include <connserver.h>
#include <userlogin.h>
#include <authenuser.h>
#include <usercli.h>

#define MAX_GRP_LEN 64
#define MAX_USER_NAME_LEN 64
#define MAX_PSWD_LEN 64

#define MAX_RETRY 4

/*******************Winsock************************************/
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
/*
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
*/

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main (int argc, char* argv[]) {
	
	if ( argc != 3  ) {
		//ClientHelp();
		printf ("Abnoraml arguments!\n");
		exit(-1);
	}

#ifdef __WINDOWS
	/*
	 * Initialize winsock
	 */
    WSADATA wsaData; //structure containing winsock impl information
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    /******************winsock server address****************/
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
	//int server_port = atoi(argv[2]);
    //iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    /******************End of winsock server address****************/

	/******************Connect with server via winsock**************************/
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
    	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
	/******************End of Connect with server via winsock**************************/
#endif

#ifdef __LINUX
	struct hostent* server_ip = gethostbyname(argv [1]);
	int server_port = atoi(argv[2]);

	// Connect with server
	int client_sock ;
	int ret = 0;
	client_sock = ConnServer(server_ip,server_port) ;
	if ( client_sock< 0 ) {
		printf ("Fail to connect with server!\n");
		exit(-1);
	} else {
		printf ("Connected with server : %s,port:%d through client socket %d........ \n",server_ip->h_name,server_port,client_sock);
	}
#endif
	// user inter-action
	char user_grp [MAX_GRP_LEN] = {0};
	char user_name [MAX_USER_NAME_LEN] = {0};
	char user_pswd [MAX_PSWD_LEN] = {0};

	// return value decleration
	int retry_cnt=0,ret=0;
	
	// User authentication
	//while (retry_cnt<MAX_RETRY) {
	do 
	{
		ret = UserLogin(user_grp,user_name,user_pswd);
		if (ret < 0) {
			printf ("Abnormal User Inputs, Exit!\n");
			exit(-1);
		}
		
		printf ("\n");
		
		printf ("Authening with server........\n");
		// authentication with server
		/*
		ret = AuthenUser(client_sock,user_grp,user_name,user_pswd);
		*/
		ret = AuthenUser(ConnectSocket,user_grp,user_name,user_pswd);
		if ( ret<0) {
			//printf ("Fail to authentication, please double check user information!\n");
			printf ("please double check and retry!\n");
			if ( retry_cnt== (MAX_RETRY-1)) {
#ifdef __LINUX
				close(ConnectSocket);
#endif
#ifdef __WINDOWS
				/***********Winsock cleanup****************/
		        closesocket(ConnectSocket);
		        WSACleanup();
#endif
				exit(-1);
			}
		} else {
			printf ("Authen success(grp:%s,user name: %s)\n",user_grp,user_name);
			printf("----------------------------\n");
			printf("\n");
			break;

		} 
		printf("----------------------------\n");
		retry_cnt++;
	} while (retry_cnt<MAX_RETRY) ;
			
	// user commands
	ret = UserCli (ConnectSocket,user_grp,user_name,user_pswd);
	printf ("Thank you. Bye!\n");
	return 0;	
}


