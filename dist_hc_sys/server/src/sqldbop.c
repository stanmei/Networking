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
  	printf ("created new db: %s.....\n",h_db_name);
  	 
	ExecSql("use myhealthdb;");	

	mysql_close(h_conn);
	printf ("Initialized database done!\n");
}

void CreateUserTbl() {
	//query whether table exist
	sprintf(sql_buf,"show tables;");
	ExecSql(sql_buf);
	
	h_res = mysql_store_result(h_conn);
	int num_rows = mysql_num_rows(h_res); 
	
	if (num_rows==0) {
		// Create user table
		ExecSql("create table users(grp varchar(24) not null unique,name varchar(24) not null unique,password char(20) not null,create_time datetime not null);");  
	
		sprintf(sql_buf,"insert into users values('admin','admin','123456');");  
		ExecSql(sql_buf);
		
		// Create patient record table
		ExecSql("create table patients(name varchar(24) not null unique,password char(20) not null,records char(256) not null, insurance_coverable char(20) not null,create_time datetime not null,);");  
	
	}
	
	mysql_free_result(h_res);
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

