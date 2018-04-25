/* File sqldbop.c
 *
 * Author : grp1
 *
 * Desc : sql database opeartions.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>

#include <cJSON.h>
#include <sqldbop.h>
#include <mysql/mysql.h>

#define MAX_ROW_MSG_LEN 256

MYSQL* h_conn ; // my sql connection
MYSQL_RES* h_res ; // my sql record
MYSQL_ROW h_row ; // my sql row

char* h_host = "localhost";
char* h_user = "root" ;
char* h_pswd = "123456" ;
char* h_db_name = "myhealthdb" ;

char  sql_buf [MAX_SQL_BUF_SIZE] ;


int ExecSql(const char * sql_buf) {
	if(mysql_real_query(h_conn,sql_buf,strlen(sql_buf)))  
        	return -1;  
    return 0;  
}

void CreateDb() {
	
	//int ret =0 ;
	// init sql connection
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-1);
	}
	
	// connect with sql database to check whetehr already existed.
	//int db_existed =0;
	if ( mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) != NULL) {	
			printf ("DB: %s already existed,skip creatation!\n",h_db_name);
			mysql_close(h_conn);
			return; 
	}
	
	// Not existed yet. create new db.
	if ( mysql_real_connect(h_conn,"127.0.0.1","root","123456","mysql",0,NULL,0) == NULL) {	
		fprintf(stderr, "Unable connect to mysql db: %s\n",mysql_error(h_conn));
		mysql_close(h_conn);
		exit (-1);
	} 
		
	// initilized new database	
	if (mysql_query(h_conn, "CREATE DATABASE myhealthdb")) 
	{
    		fprintf(stderr, "%s\n", mysql_error(h_conn));
	    	mysql_close(h_conn);
    		exit(-1);
  	}

	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

  	printf ("created new db: %s.....\n",h_db_name);

	mysql_close(h_conn);
	printf ("Initialized database done!\n");
}

void CreateUserTbl() {
	// init sql connection
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection[new user tbl]!\n");
		exit(-1);
	}

	if ( mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) == NULL) {	
			printf ("DB: %s could not be reached!\n",h_db_name);
			mysql_close(h_conn);
			return; 
	}

	//query whether table exist
	sprintf(sql_buf,"use myhealthdb;");
	ExecSql(sql_buf);
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

	sprintf(sql_buf,"DROP TABLE IF EXISTS users;");
	ExecSql(sql_buf);
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);
	
/*
	h_res = mysql_store_result(h_conn);
   	fprintf(stderr, "mysql check user table exists: %s\n", mysql_error(h_conn));
	//printf("myhealthdb for table check!\n");
	int num_rows = mysql_num_rows(h_res); 

	printf("Check whether new user table need to be created, row num: %d .....\n",num_rows);
	
	if (num_rows==0) {
*/
	printf("Creating user table......\n");
	// Create user table
	//ExecSql("CREATE TABLE users(grp varchar(24) not null unique,name varchar(24) not null unique,password char(20) not null);");  
	ExecSql("CREATE TABLE users(grp varchar(24) not null,name varchar(24) not null,password char(20) not null);");  
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);
	
	ExecSql("INSERT INTO users VALUES('admin','admin','123456');");
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);
		
	// Create patient record table
	ExecSql("DROP TABLE IF EXISTS patients;");
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

	ExecSql("CREATE TABLE patients(name varchar(24) not null unique,password char(20) not null,records char(32) not null, insurance_coverable char(20) not null);");  

	printf("New user table be created !\n");
	
/*
	}
	
	mysql_free_result(h_res);
*/
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);
	mysql_close(h_conn);
}

// user authentication
int Authenticate(char* grp,char* name, char* passwd) {
	// init sql connection
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-3);
	}
	
	// connect with sql database
	if (!mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) ) {
		printf ("Failed to connect to sql db: %s",h_db_name);
		exit (-3);
	}
	printf ("Connected with sql db: %s\n",h_db_name);
	
	// initilized new database
    	sprintf(sql_buf, "SELECT password, grp from users where name = '%s' for update;", name);
    	printf("Authen cmd in sql buffer: %s \n", sql_buf);

    if (mysql_query(h_conn, sql_buf)) {
    	printf("Authen: mysql query fail! \n");          
    }

    h_res = mysql_store_result(h_conn);

    int row_num = mysql_num_rows(h_res);

    printf("Authen - query returned row numbers : %d \n", row_num);

    // returned row elements including: password, grp
    h_row = mysql_fetch_row(h_res);
    if( h_row == NULL) {
        printf("Authen- fetched invalid row! \n");
        return -3;
    }
    
    if (row_num > 1) {
    	printf ("Authen- duplicated user be found: %s. \n",name);
    	return -4 ;
    }
    
    if ( strcmp(h_row[0],passwd) ) {
    	printf ("Authen - unmatched password: %s : %s \n", h_row[0],passwd);
    	return -1;
    }
    
    if ( strcmp(h_row[1],grp)) {
    	printf ("Authen - unmatched grp : %s : %s \n", h_row[1],grp);
    	return -2;
    }
    
    // close sql connection
    mysql_free_result(h_res);
    mysql_close(h_conn);
    
    return 0;
}

// user account create
int Create_Accnt (char* grp,char* new_name, char* new_passwd) {
	int ret =0 ;
	/*
	 * init sql connection
	 */
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-3);
	}
	
	// connect with sql database
	if (!mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) ) {
		printf ("Failed to connect to sql db: %s",h_db_name);
		exit (-3);
	}
	/* Query whether exist
	 */
  	sprintf(sql_buf, "SELECT * from users where name = '%s' for update;", new_name);
    	
    	if (mysql_query(h_conn, sql_buf)) {
    		printf("Authen: mysql query fail! \n");          
    	}

    	h_res = mysql_store_result(h_conn);
    	int row_num = mysql_num_rows(h_res);	
	
    	// close sql connection
   	mysql_free_result(h_res);
	if (row_num==0)
	{ 
	/* Insert new account
	 */
		sprintf(sql_buf,"use myhealthdb;");
		ret= ExecSql(sql_buf);
		h_res = mysql_store_result(h_conn);
    		// close sql connection
   		mysql_free_result(h_res);

		sprintf(sql_buf,"INSERT INTO users VALUES('%s','%s','%s');",grp,new_name,new_passwd);
		ret= ExecSql(sql_buf);
		h_res = mysql_store_result(h_conn);
    		// close sql connection
   		mysql_free_result(h_res);	
	//ExecSql("INSERT INTO users VALUES('admin','admin','123456');");
	} else {
		ret = 1 ;
		printf("The account to be created already existed!\n");
	}
    /*
     * close sql connection
     */   
    mysql_close(h_conn);    
    return ret;

}

// delete account
int Delete_Accnt(char* name) {
	int ret = 0 ;
    	/*
	 * init sql connection
     	 */
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-3);
	}
	
	// connect with sql database
	if (!mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) ) {
		printf ("Failed to connect to sql db: %s",h_db_name);
		exit (-3);
	}
	printf ("Connected with sql db: %s\n",h_db_name);
	
    	/*
	 * initilized new database
     	 */
	sprintf(sql_buf,"use myhealthdb;");
	ret= ExecSql(sql_buf);
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

    	sprintf(sql_buf, "SELECT * from users where name = '%s';", name);
    	if (mysql_query(h_conn, sql_buf)) {
    		printf("[FAIL] Delete: The user (%s) is not existed in mysql! \n",name);          
		ret = -1 ;
  	  }
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

    	//h_res = mysql_store_result(h_conn);
    	//int row_num = mysql_num_rows(h_res);

    	sprintf(sql_buf, "DELETE from users where name = '%s';", name);
    	if (mysql_query(h_conn, sql_buf)) {
    		//printf("[FAIL] Delete: The user (%s) delete unsuccessfully! \n",name);          
		fprintf(stderr,"%s \n",mysql_error(h_conn));		
		ret = -2 ;
  	}
	else {
		printf("server: delete user (%s) from table users.\n",name);
	}
      	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);
    	mysql_close(h_conn);
    
    	return ret ;//row_num ; //return matched rows
}

// Update account
int Update_Item (char* name,char* new_item_val) {
	int ret = 0 ;
    	/*
	 * init sql connection
     	 */
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-3);
	}
	
	// connect with sql database
	if (!mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) ) {
		printf ("Failed to connect to sql db: %s",h_db_name);
		exit (-3);
	}
	printf ("Connected with sql db: %s\n",h_db_name);
	
    	/*
	 * initilized new database
     	 */
    	sprintf(sql_buf, "SELECT * from users where name = '%s';", name);
    	if (mysql_query(h_conn, sql_buf)) {
    		printf("[FAIL] Delete: The user is not existed in mysql! \n");          
		ret = -1 ;
  	  }
	h_res = mysql_store_result(h_conn);
    	// close sql connection
   	mysql_free_result(h_res);

    	//h_res = mysql_store_result(h_conn);
    	//int row_num = mysql_num_rows(h_res);

    	sprintf(sql_buf, "UPDATE users SET password ='%s' where name = '%s';", new_item_val,name);
   	if (mysql_query(h_conn, sql_buf)) {
    		printf("[FAIL] Update : The update item is not success in mysql! \n");          
		ret = -2 ;
  	  }
	 else {
		printf ("Update password for user (%s). \n",name);
	}

    	h_res = mysql_store_result(h_conn);
    	// close sql connection
    	mysql_free_result(h_res);
    	mysql_close(h_conn);
    
    	return ret ;//row_num ; //return matched rows
}

// Update account
int Query_Tbl (char* tbl,int* tbl_row_num,char* qry_rslt_rows[]) {
	int ret = 0 ;
    	/*
	 * init sql connection
     	 */
	h_conn = mysql_init(NULL);  	
	if (h_conn==NULL) {
		printf ("Failed to init sql connection!\n");
		exit(-3);
	}
	
	// connect with sql database
	if (!mysql_real_connect(h_conn,h_host,h_user,h_pswd,h_db_name,0,NULL,0) ) {
		printf ("Failed to connect to sql db: %s",h_db_name);
		exit (-3);
	}
	printf ("Connected with sql db: %s\n",h_db_name);
	
    	/*
	 * Query table
     	 */
    	sprintf(sql_buf, "SELECT * from %s;", tbl);

    	if (mysql_query(h_conn, sql_buf)) {
    		printf("[FAIL] Query: The table %s is not existed in mysql! \n",tbl);          
		ret = -1 ;
		return ret;
  	  }

    	h_res = mysql_store_result(h_conn);
    	int row_num = mysql_num_rows(h_res);
	int num_fields = mysql_num_fields(h_res);

	int idx=0;
	while((h_row = mysql_fetch_row(h_res))){
		char qry_row_msg [MAX_ROW_MSG_LEN] ;
		memset(qry_row_msg,0,MAX_ROW_MSG_LEN);
		
		for (int i=0; i<num_fields; i++) {
			strcat(qry_row_msg,h_row[i]);
			strcat(qry_row_msg,"  ");
		}
		printf("%s. \n",qry_row_msg);
//		strncpy(qry_rslt_rows[idx],qry_row_msg,strlen(qry_row_msg)); //!segmentation error. -> wrong, this is pointer not arrary.
		//qry_rslt_rows[idx] = qry_row_msg; -> wrong, pointer be stored.
		sprintf(qry_rslt_rows[idx],"%s",qry_row_msg);
	        idx++;	
	}
 
/*
     for (int i=0; i<row_num;i++)
	printf ("array elements %d : %s \n",i,qry_rslt_rows[i]);
*/
    *tbl_row_num = row_num;	
    // close sql connection
    mysql_free_result(h_res);
    mysql_close(h_conn);
    
    return ret ;//row_num ; //return matched rows
}
