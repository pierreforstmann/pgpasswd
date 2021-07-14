/*
 *	pgpasswd.c
 *	
 *	change PostgreSQL account password	
 *
 *	Copyright (c) 2021 Pierre Forstmann 
 *	        
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "postgres.h"
#include "port.h"
#include "getopt_long.h"
#include "libpq-fe.h"

#define CONNECT_STRING_MAX_LENGTH	100
#define PASSWORD_MAX_LENGTH		30	
#define STMT_MAX_LENGTH			60

/*
 * static functions
 */
static void print_clientlibversion();
static void print_serverlibversion(PGconn *conn);
static void print_conninfo(char *s);
static void exit_nicely(PGconn *conn, PGresult *res);
static void print_stmt(char *s);

static void print_clientlibversion()
{
	int lib_ver = PQlibVersion();

	printf("libpq version: %d\n", lib_ver);
	
}

static void print_serverlibversion(PGconn *conn)
{
	int ver = PQserverVersion(conn);

	printf("Server version: %d\n", ver);

}
static void print_conninfo(char *s)
{
	printf("conninfo=%s \n", s);
}

static void exit_nicely(PGconn *conn, PGresult *res) 
{
	if (res != NULL)
		PQclear(res);
	if (conn != NULL)
		PQfinish(conn);

	exit(1);
}
static void print_stmt(char *s)
{
	printf("stmt=%s \n", s);
}

/*
 * main
 */

int main(int argc, char **argv) 
{

	char	conninfo[CONNECT_STRING_MAX_LENGTH];
	char	old_password[PASSWORD_MAX_LENGTH];
	char	new_password1[PASSWORD_MAX_LENGTH];
	char	new_password2[PASSWORD_MAX_LENGTH];
	PGconn 	*conn;
	char	stmt[STMT_MAX_LENGTH];
	PGresult	*res;
	bool	verbose;

	static struct option long_options[] = 
	{
		{"dbname", required_argument, NULL, 'd'},
		{"host", required_argument, NULL, 'h'},
		{"port", required_argument, NULL, 'p'},
		{"username", required_argument, NULL, 'U'},
		{"verbose", no_argument, NULL, 'v'},
		{NULL, 0, NULL, 0}
	};

	int c;
	int optindex = 0;

	verbose = false;

	conn = NULL;
	res = NULL;
	conninfo[0]='\0';

	if (verbose == true)
		print_clientlibversion();

	while (( c = getopt_long(argc, argv, "d:h:p:vU:", long_options, &optindex)) != -1)
	{
		switch (c)
		{
			case 'd':
				strcat(conninfo,"dbname=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				break;
			case 'h':
				strcat(conninfo,"host=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				break;
			case 'p':
				strcat(conninfo,"port=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				break;
			case 'U':
				strcat(conninfo,"user=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				break;
			case 'v':
				verbose = true;
				break;
			default:
				fprintf(stderr, "Try \"pgpasswd --help\" for more information.\n");
				exit_nicely(conn, res);
				break;
		}
	}
	

	simple_prompt("Password:", old_password, PASSWORD_MAX_LENGTH, false);	
	strcat(conninfo,"password=");
        strcat(conninfo, old_password);
        strcat(conninfo, " ");

	if (verbose == true)
		print_conninfo(conninfo);

	conn = PQconnectdb(conninfo);

	if (PQstatus(conn) == CONNECTION_BAD) 
	{ 
        	fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        	exit_nicely(conn, res);
	} 

	if (verbose == true)
		print_serverlibversion(conn);

	simple_prompt("New password:", new_password1, PASSWORD_MAX_LENGTH, false);	
	simple_prompt("New password:", new_password2, PASSWORD_MAX_LENGTH, false);	
	if (strcmp(new_password1, new_password2) != 0)
	{
		fprintf(stderr, "New passwords do not match.\n");
		exit_nicely(conn, res);
	}

	strcpy(stmt, "ALTER USER ");
	strcat(stmt, PQuser(conn)); 
	strcat(stmt, " PASSWORD '");
	strcat(stmt, new_password2); 
	strcat(stmt, "'");
	if (verbose == true)
		print_stmt(stmt);
	res = PQexec(conn, stmt);
    
    	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "Password change failed: %s \n", PQerrorMessage(conn));
        	exit_nicely(conn, res);
    	}
	
	printf("Password changed.\n");

	PQfinish(conn);

	return 0;
}

