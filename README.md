# pgpasswd
Change or reset PostgreSQL account password from Linux command line without entering any SQL statement and sending the **encrypted** password to server.<br>

`chpasswd` and `resetpasswd`  have been successfully tested with PostgreSQL 14, 15, 16, 17 and 18 on Debian 13 and RHEL 9.8.

## Installation

`pgpasswd` provides new PostgreSQL client executables:  `chpasswd` and `resetpasswd` are standalone executables (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module can be built using the standard PGXS infrastructure. 
It can also be built using a standard PostgreSQL server installation that includes the corresponding PG development package to be able to compile libpq code:
for example to compile with PG 17 on RHEL-like systems `postgresql17-devel` RPM must be installed.

In both cases the `pg_config` program must be available in your $PATH.

```
git clone https://github.com/pierreforstmann/pgpasswd.git 
cd pgpasswd
make 
```
## Installing 
`make install` installs `chpasswd` and `resetpasswd` to PostgreSQL binaries directory returned by:
```
pg_config --bindir
```

You need to set LD_LIBRARY_PATH for ``` libpq```:
```export LD_LIBRARY_PATH=<lib directory of PosgreSQL binaries>:$LD_LIBRARY_PATH```

## Usage

`chpasswd` for PostgreSQL can be compared to `passwd` on Linux and UNIX systems when used to change current user password.

Enter:
* same parameters (host name, port number, user account and database name) as `psql` to connect to PostgreSQL 
* current user password once
* current user new password twice.

Example:

``` 
chpasswd -h localhost -p 5432 -U test -d postgres
Password:
New password:
Confirm new password:
Password changed.
```

Detailed PG client, PG server versions and passwords can be displayed with -v option after successfull connexion:
```
$ ./chpasswd -h localhost -p 5432 -d postgres -U test -v
Password:
PG libpq version: 170004
status:CONNECTION_OK
conninfo: host=localhost port=5432 dbname=postgres user=test password=test  
PG server version: 170004
New password:
Confirm new password:
ALTER USER test PASSWORD 'SCRAM-SHA-256$4096:oHFXEfD/ol1RTHvsUCjPMQ==$+yZF9VQO3vlq+ypOA+Fbyv6oao22VoQQx0gHHR5gyeQ=:+vELUZddKLWUDMxNAPOD9IW8W6pl5eAt9bcky1XkbvI='
Password changed.
```

`resetpasswd` for PostgreSQL can be compared to `passwd` on Linux and UNIX systems when used to change another user password:

Enter:
* same parameters (host name, port number, superuser account and database name) as `psql` to connect to PostgreSQL
* superuser password
* other user account for which password must be reset 
* other user account new password twice.

Example:

``` 
resetpasswd -h localhost -p 5432 -U admin -d postgres
Password:
Account:test
New password:
Confirm new password:
Password successfully reset.
```
