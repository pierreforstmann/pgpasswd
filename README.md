# pgpasswd
Change PostgreSQL account password from Linux command line without entering any SQL statement.<br>

`pgpasswd`  has been successfully tested with PostgreSQL 9.5, 9.6, 10, 11, 12, 13, 14, 15 and 16 on Linux. <br>
## Installation

`pgpasswd` is a PostgreSQL extension and a standalone executable (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module can be built using the standard PGXS infrastructure. 
It can also be built using a standard PostgreSQL server installation that includes the corresponding PG development package to be able to compile libpq code:
for example to compile with PG 15 on RHEL-like systems postgresql15-devel RPM must be installed.

In both cases the `pg_config` program must be available in your $PATH:

```
git clone https://github.com/pierreforstmann/pgpasswd.git 
cd pgpasswd
make 
```

`make install` installs `pgpasswd` to PostgreSQL binaries directory returned by:
```
pg_config --bindir
```

## Usage

`pgpasswd` for Postgresql can be compared to `passwd` on Linux and UNIX systems.

Enter:
* same parameters (host name, port number, user name and database name) as `psql` to connect to PostgreSQL 
* current password once
* new password twice.

Example:

``` 
pgpasswd -h localhost -p 5432 -U test -d postgres
Password:
New password:
New password:
Password changed.
```

Detailed PG client and PG server versions can be displayed with -v option after successfull connexion:
```
$ ./pgpasswd -h localhost -p 5432 -d postgres -U test -v
Password:
PG libpq version: 150006
conninfo: host=localhost port=5432 dbname=postgres user=test password=test  
PG server version: 160002
New password:
```
