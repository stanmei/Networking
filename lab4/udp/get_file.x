/* This XDR file is the input to rpcgen
 * Specification of get_file() which returns the file content.
 *
 */

/*
struct tran_seq {
	char filename[64];
	int seq;
}; 
*/

program GETFILEPROG { 

	version GETFILE_VER { /* declaraton of program version*/
	string GET_FILE_CONTENT(string,int) = 1; /* procedure number*/
	int GET_FILE_TRANS(string) = 2; /* procedure number*/
	} = 1; /*definition of program version*/
} = 0x2000000; /* remote program number (be unique)*/
