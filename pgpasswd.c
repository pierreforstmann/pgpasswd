/*
 *	pgpasswd.c
 *	
 *	change PostgreSQL account password	
 *
 *	Copyright (c) 2021, 2022, 2023, 2024 Pierre Forstmann 
 *	        
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <getopt.h>
#include <termios.h>
#include "libpq-fe.h"

#define no_argument			0
#define required_argument		1

#define CONNECT_STRING_MAX_LENGTH	128
#define PASSWORD_MAX_LENGTH		64	
#define STMT_MAX_LENGTH			512	

/*
 * static functions
 */
static void usage();
static void print_clientlibversion();
static void print_serverlibversion(PGconn *conn);
static void print_conninfo(char *s);
static void exit_nicely(PGconn *conn, PGresult *res);

static void print_clientlibversion()
{
	int lib_ver = PQlibVersion();

	printf("PG libpq version: %d\n", lib_ver);
	
}

static void print_serverlibversion(PGconn *conn)
{
	int ver = PQserverVersion(conn);

	printf("PG server version: %d\n", ver);

}
static void print_conninfo(char *s)
{
	printf("conninfo: %s \n", s);
}

static void exit_nicely(PGconn *conn, PGresult *res) 
{
	if (res != NULL)
		PQclear(res);
	if (conn != NULL)
		PQfinish(conn);

	exit(1);
}

static void usage(void)
{
	printf(("pgpasswd changes PostgreSQL account password.\n\n"));
	printf(("Usage:\n"));
	printf(("  pgpasswd [OPTION]...\n\n"));
	printf(("Options:\n"));
	printf(("  -h, --host       instance host name\n"));
	printf(("  -p, --port       instance port number\n"));
	printf(("  -d, --dbname     database name\n"));
	printf(("  -U, --user       user name\n"));
	printf(("  -v, --verbose    verbose mode\n"));
	printf(("\n"));
}

/*
 *
 * read_password 
 * 
 */
static void read_password(char password[])
{
    static struct termios oldt, newt;
    int i = 0;
    int c;

    /*
     * saving the old settings of STDIN_FILENO and copy settings for resetting
     */
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;

    /*
     * setting the approriate bit in the termios struct
     */
    newt.c_lflag &= ~(ECHO);          

    /*
     * setting the new bits
     */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*
     * reading the password from the console
     */
    while ((c = getchar())!= '\n' && c != EOF && i < PASSWORD_MAX_LENGTH){
        password[i++] = c;
    }
    password[i] = '\0';

    /*
     * resetting our old STDIN_FILENO
     */ 
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

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
	PGresult	*res;
	bool	verbose;
	bool	host_is_set;
	bool	port_is_set;
	bool	user_is_set;
	bool	database_is_set;
	char	stmt[STMT_MAX_LENGTH];
	char	*encrypted_new_password;

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

	if (argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
		{
			usage();
			exit(0);
		}
	}

	host_is_set = false;
	port_is_set = false;
	user_is_set = false;
	database_is_set = false;

	while (( c = getopt_long(argc, argv, "d:h:p:vU:", long_options, &optindex)) != -1)
	{
		switch (c)
		{
			case 'd':
				strcat(conninfo,"dbname=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				database_is_set = true;
				break;
			case 'h':
				strcat(conninfo,"host=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				host_is_set = true;
				break;
			case 'p':
				strcat(conninfo,"port=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				port_is_set = true;
				break;
			case 'U':
				strcat(conninfo,"user=");
				strcat(conninfo, optarg);
				strcat(conninfo, " ");
				user_is_set = true;
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

	if (host_is_set == false || port_is_set == false || database_is_set == false || user_is_set == false)
	{
        	fprintf(stderr, "Missing parameters \n\n");
		usage();
        	exit_nicely(conn, res);
	}

	printf("Password:");
	read_password(old_password);
	printf("\n");
	strcat(conninfo,"password=");
        strcat(conninfo, old_password);
        strcat(conninfo, " ");

	if (verbose == true) {
		print_clientlibversion();
	}


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

	printf("New password:");
	read_password(new_password1);
	printf("\n");
	printf("Confirm new password:");
	read_password(new_password2);
	printf("\n");
	if (strcmp(new_password1, new_password2) != 0)
	{
		fprintf(stderr, "New passwords do not match.\n");
		exit_nicely(conn, res);
	}

#if PG_VERSION_NUM >= 17000 
	/*
	 * relies on password_encryption from server
	 */
	res = PQchangePassword(conn, PQuser(conn), new_password2);
#else
	encrypted_new_password = PQencryptPasswordConn(conn, new_password2, 
			                           PQuser(conn), NULL);
	strcpy(stmt, "ALTER USER ");
	strcat(stmt, PQuser(conn)); 
	strcat(stmt, " PASSWORD '");
	strcat(stmt, encrypted_new_password); 
	strcat(stmt, "'");
	/* 
	if (verbose == true)
		print_stmt(stmt);
	*/
	res = PQexec(conn, stmt);
#endif

    
    	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "Password change failed: %s \n", PQerrorMessage(conn));
        	exit_nicely(conn, res);
    	}
	
	printf("Password changed.\n");

	PQfinish(conn);

	return 0;
}

