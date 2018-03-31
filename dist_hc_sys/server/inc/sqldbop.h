#ifndef _SQLDBOP_H
#define _SQLDBOP_H

#define MAX_SQL_BUF_SIZE 1024

// exec sql commands
int ExecSql(const char * sql_buf) ;

// Select database, create it if doesn't existed.
void CreateDb() ;
void CreateUserTbl() ;

// user authentication
int Authenticate(char* grp,char* name, char* passwd) ;

/*
 * Account commands
 */

// change password
//int ChangePswd(char* grp, char* name, char* old_pswd, char* new_pswd);

// Create account 
int CreateAccnt(char* grp, char* name, char* ini_pswd);
// Delete account 
//int DeleteAccnt(char* grp, char* name, char* pswd);

/*
 * User operations
 */
//int QryRcord(MYSQL* conn, char* patient_name);

/*
 * Debugging
 */

void PrintUserTbl ();


#endif
