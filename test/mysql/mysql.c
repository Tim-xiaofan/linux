/** 20210324 14:21, zyj ,GuangDong*/
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#define N 3
#define M 256
#define SQLLIM 4096

#define DATABASE "school"
#define TABLE "student"
#define USER "zyj"
#define PASSWD "zyj"

static const char values[N][M] = {
	"(0, 'Tom', 'B', '2021-7-21', '12345678901', 'US', 'PE')",
	"(1, 'Jerry', 'G', '2021-8-21', '1234567910', 'UK', 'PE')",
	"(2, 'Spike', 'B', '2021-7-2', '12345668901', 'CN', 'PE')",
};

static const char fields[SQLLIM] = {
	"id integer not null,"
	"name varchar(20) not null,"
	"sex char,"
	"birth datetime not null,"
	"tel char(11),"
	"addr varchar(255),"
	"major varchar(255),"
	"primary key (id)"
};

static const char *fmts[M] = {"%s\t","%s\t","%s\t","%s\t","%s\t","%s\t","%s"};
static const char *fmts1[M] = {"%s\t","%s\t","%s\t","%s\t\t\t","%s\t\t","%s\t","%s"};

static void show_result(MYSQL *mysql);
static void query_error(int ret); 

int main()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int ret;
	char sql[SQLLIM];
	
	if(!mysql_init(&mysql))
	{
		fprintf(stderr, "failed to init mysql struct\n");
		exit(-1);
	}

	/** connected to database*/
	if(!mysql_real_connect(
				&mysql,
				"localhost",
                USER,
                PASSWD,
                DATABASE,
                0,
                NULL,
                0))
	{
		fprintf(stderr,
					"failed to connect to server.  ERROR : %s\n", 
					mysql_error(&mysql));
		exit(-1);
	}

	printf("connect success.\n");

	/** drop table if exist*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "drop table if exists %s", TABLE);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);


	/** create table*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "create table %s (%s)", TABLE, fields);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);

	if(!(res = mysql_list_tables(&mysql, NULL)))
	{
		fprintf(stderr,
					"failed to list tables.  ERROR : %s\n", 
					mysql_error(&mysql));
	}
	else
	{
		printf("table count : %lu\n", res->row_count);
		while((row = mysql_fetch_row(res))) 
		{
			printf("%s\n", *row);
			row = mysql_fetch_row(res);
		}
	}
	mysql_free_result(res);

	/** insert first*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "insert into %s values %s", TABLE, values[0]);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);

	printf("after inserting\n");
	memset(sql, 0, SQLLIM);
	sprintf(sql, "select * from %s", TABLE);
	mysql_query(&mysql, sql);
	show_result(&mysql);

	/** insert remaining*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "insert into %s values %s, %s", TABLE, values[1], values[2]);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);

	printf("after inserting\n");
	memset(sql, 0, SQLLIM);
	sprintf(sql, "select * from %s", TABLE);
	mysql_query(&mysql, sql);
	show_result(&mysql);

	/** update*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "update %s set sex = 'B' where name = 'Jerry'", TABLE);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);

	printf("after updating\n");
	memset(sql, 0, SQLLIM);
	sprintf(sql, "select * from %s", TABLE);
	mysql_query(&mysql, sql);
	show_result(&mysql);
	
	/** delete*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "delete from %s where name = 'Spike'", TABLE);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	//printf("ret : %d\n", ret);
	if(ret != 0) 
	  query_error(ret);

	printf("after deleting\n");
	memset(sql, 0, SQLLIM);
	sprintf(sql, "select * from %s", TABLE);
	mysql_query(&mysql, sql);
	show_result(&mysql);

	/** delete all*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "delete from %s where id >= 0", TABLE);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);

	/** drop table*/
	memset(sql, 0, SQLLIM);
	sprintf(sql, "drop table %s", TABLE);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);

	/** close connect*/
	printf("Type enter to exit.");
	gets();
	mysql_close(&mysql);
}

void query_error(int ret)
{
	switch(ret)
	{
		case CR_COMMANDS_OUT_OF_SYNC:
			fprintf(stderr, "Commands were executed in an improper order.\n");
			break;
		case CR_SERVER_GONE_ERROR :
			fprintf(stderr, "The MySQL server has gone away.\n");
			break;
		case CR_SERVER_LOST :
			fprintf(stderr, "The connection to the server was lost during the query.\n");
			break;
		case CR_UNKNOWN_ERROR :
			fprintf(stderr, "An unknown error occurred.\n");
			break;
		default:
			fprintf(stderr, "error with id %d\n", ret);
	}
}


void show_result(MYSQL *mysql)
{
	MYSQL_RES * res;
    MYSQL_FIELD * field;
    MYSQL_ROW row;
	int i;

    if(!(res = mysql_store_result(mysql)))
	{/** retrieves a complete result set to the client.*/
		fprintf(stderr,
					"failed to retrieves result set.  ERROR : %s\n", 
					mysql_error(mysql));
		return;
	}

	/** output field names */
	i = 0;
    while((field = mysql_fetch_field(res)))
        printf(fmts1[i++], field->name);
    printf("\n");

	/** output result set*/
	while((row = mysql_fetch_row(res))) /** Fetches the next row from the result set. */
	{
		int i;
		for(i = 0; i < res->field_count; i++)
		{
			printf(fmts[i], row[i]);
		}
		printf("\n");
	}
    mysql_free_result(res);/** Frees memory used by a result set.*/
}
