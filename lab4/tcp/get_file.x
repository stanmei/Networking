/* This XDR file is the input to rpcgen
 * Specification of get_file() which returns the file content.
 *
 */

program GETFILEPROG { 

	version GETFILE_VER { /* declaraton of program version*/
	string GET_FILE_CONTENT(string filename) = 1; /* procedure number*/

	} = 1; /*definition of program version*/

} = 0x2fffffff; /* remote program number (be unique)*/
