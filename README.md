# pgpasswd
Change or reset PostgreSQL account password from Linux command line without entering any SQL statement and sending the **encrypted** password to server.<br>

`chpasswd` and `resetpasswd`  have been successfully tested with PostgreSQL 14, 15, 16, 17 and 18 on Debian 13 and RHEL 9.8.

## Installation

`pgpasswd` provides new PostgreSQL client executables:  `chpasswd` and `resetpasswd` are standalone executables (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module can be built using a standard PostgreSQL server installation that includes the corresponding PG development package to be able to compile PostgreSQL client library code.
- `pg_config` program must be available in $PATH for user running make.
- user running make needs also sudo privilege in order to be able to install executables in PostgreSQL target directory (output of ```pg_config --bindir```).

### RHEL 9.8
```
sudo dnf -y install postgresql17-devel
export PATH=/usr/pgsql-17/bin:$PATH
git clone https://github.com/pierreforstmann/pgpasswd.git
cd pgpasswd
make clean
make
```

### Debian 13

```
sudo apt -y install libpq-dev
export PATH=/usr/lib/postgresql/18/bin/pg_config:$PATH
git clone https://github.com/pierreforstmann/pgpasswd.git
cd pgpasswd
make clean
make
```

### Installing
```
make install
```
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
