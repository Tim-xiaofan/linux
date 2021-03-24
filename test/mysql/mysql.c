/** 20210324 14:21, zyj ,GuangDong*/
#include <stdio.h>
#include <string.h>
#include <mysql.h>

#define N 3
#define M 256

#define DATABASE "school"
#define TABLE "student"
#define USER "zyj"
#define PASSWD "zyj"

static const char values[N][M] = {
	"(0, 'Tom', 1, '2021-7-21', '12345678901', 'US', 'PE')",
	"(1, 'Jerry', 1, '2021-8-21', '123477678901', 'UK', 'PE')",
	"(2, 'Spike', 1, '2021-7-2', '12345668901', 'CN', 'PE')",
};

void show_result(MYSQL *mysql);
void query_error(int ret);

int main()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int ret;
	
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

	if(!(res = mysql_list_tables(&mysql, NULL)))
	{
		fprintf(stderr,
					"failed to list tables.  ERROR : %s\n", 
					mysql_error(&mysql));
	}
	else
	{
		printf("table count : %lu\n", res->row_count);
		row = mysql_fetch_row(res);
		while((row = mysql_fetch_row(res))) 
		{
			printf("%s\n", *row);
			row = mysql_fetch_row(res);
		}
	}
	mysql_free_result(res);

	/** insert */
	char sql[M];
	sprintf(sql, "insert into %s valus %s", TABLE, values[0]);
	printf("\nexcute sql : %s\n", sql);
	ret = mysql_query(&mysql, sql);
	if(ret != 0) query_error(ret);

	printf("after excuting\n");
	show_result(&mysql);

	/** close connect*/
	mysql_close(&mysql);
}

void query_error(int ret)
{
	switch(ret)
	{
		case CR_COMMANDS_OUT_OF_SYNC:
			fprintf(stderr, "Commands were executed in an improper order.\n");
		case CR_SERVER_GONE_ERROR :
			fprintf(stderr, "The MySQL server has gone away.\n");
		case CR_SERVER_LOST :
			fprintf(stderr, "The connection to the server was lost during the query.\n");
		case CR_UNKNOWN_ERROR :
			fprintf(stderr, "An unknown error occurred.\n");
	}
}


void show_result(MYSQL *mysql)
{
	MYSQL_RES * res;
    MYSQL_FIELD * field;
    MYSQL_ROW row;

    if(!(res = mysql_store_result(mysql)))
	{/** Retrieves a complete result set to the client.*/
		printf("failed to get result set\n");
		return;
	}

	/** output field names */
    while((field = mysql_fetch_field(res)))
        printf("%s\t", field->name);
    printf("\n");

	/** output result set*/
	while((row = mysql_fetch_row(res))) /** Fetches the next row from the result set. */
	{
		int i;
		for(i = 0; i < res->field_count; i++)
		{
			printf("%s\t", row[i]);
		}
		printf("\n");
	}
    mysql_free_result(res);/** Frees memory used by a result set.*/
}
