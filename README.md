# pgpasswd
Change PostgreSQL account password from Linux command line without entering any SQL statement and sending the encrypted password to server.<br>

`pgpasswd`  has been successfully tested with PostgreSQL 17 on Linux. <br>
## Installation

`pgpasswd` is a PostgreSQL extension and a standalone executable (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module can be built using the standard PGXS infrastructure. 
It can also be built using a standard PostgreSQL server installation that includes the corresponding PG development package to be able to compile libpq code:
for example to compile with PG 17 on RHEL-like systems `postgresql17-devel` RPM must be installed.

Only PostgreSQL version started at 17.0 are supported because it uses PQchangePassword function only added to libpq in 2024 to PostgreSQL 17 code.

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
Confirm new password:
Password changed.
```

Detailed PG client and PG server versions can be displayed with -v option after successfull connexion:
```
$ ./pgpasswd -h localhost -p 5432 -d postgres -U test -v
Password:
PG libpq version: 170004
conninfo: host=localhost port=5432 dbname=postgres user=test password=test  
PG server version: 170004
New password:
```
