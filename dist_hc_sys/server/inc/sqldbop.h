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
int Create_Accnt(char* grp, char* new_name, char* new_passwd);
// Delete account 
int Delete_Accnt(char* name) ;
int Update_Item (char* tbl,char* name,char* new_item,char* new_item_val) ;
int Query_Tbl (char* tbl,char* item,int* tbl_row_num,char* qry_rslt_rows[]);

/*  Healthcare operations
 */
int Create_Patient (char* new_patient_name,char* new_patient_insurance, char* new_patient_record) ;
/*
 * User operations
 */
//int QryRcord(MYSQL* conn, char* patient_name);

/*
 * Debugging
 */

void PrintUserTbl ();


#endif
