# pgpasswd
Change or reset PostgreSQL account password from Linux command line without entering any SQL statement and sending the **encrypted** password to server.<br>

`chpasswd` and `resetpasswd`  have been successfully tested with PostgreSQL 13, 14, 15, 16 and 17 on Linux with `password_encryption` set to `scram-sha-256` or to `md5`. <br>
## Installation

`pgpasswd` is a PostgreSQL extension :  `chpasswd` and `resetpasswd` are standalone executables (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module can be built using the standard PGXS infrastructure. 
It can also be built using a standard PostgreSQL server installation that includes the corresponding PG development package to be able to compile libpq code:
for example to compile with PG 17 on RHEL-like systems `postgresql17-devel` RPM must be installed.

In both cases the `pg_config` program must be available in your $PATH:

```
git clone https://github.com/pierreforstmann/pgpasswd.git 
cd pgpasswd
make 
```

`make install` installs `chpasswd` and `resetpasswd` to PostgreSQL binaries directory returned by:
```
pg_config --bindir
```

## Usage

`chpasswd` for PostgreSQL can be compared to `passwd` on Linux and UNIX systems when used to change current user password.

Enter:
* same parameters (host name, port number, user name and database name) as `psql` to connect to PostgreSQL 
* current password once
* new password twice.

Example:

``` 
chpasswd -h localhost -p 5432 -U test -d postgres
Password:
New password:
Confirm new password:
Password changed.
```

Detailed PG client and PG server versions can be displayed with -v option after successfull connexion:
```
$ ./chpasswd -h localhost -p 5432 -d postgres -U test -v
Password:
PG libpq version: 170004
conninfo: host=localhost port=5432 dbname=postgres user=test password=test  
PG server version: 170004
New password:
Password changed.
```

`resetpasswd` for PostgreSQL can be compared to `passwd` on Linux and UNIX systems when used to change another user passord:
user account used to connect to PostgreSQL must have superuser privilege. After successfull connection, `resetpasswd' prompts
for user account for which password must be changed: only new password for this user account must be entered.

Example:

``` 
resetpasswd -h localhost -p 5432 -U admin -d postgres
Password:
Account:test
New password:
Confirm new password:
Password successfully reset.
```
