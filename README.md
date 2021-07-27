# pgpasswd
Change PostgreSQL account password from Linux command line without entering any SQL statement.<br>

`pgpasswd`  has been successfully tested with PostgreSQL 9.5, 9.6, 10, 11, 12 and 13 on Linux. <br>
## Installation

`pgpasswd` is a PostgreSQL extension and a standalone executable (no need to install anything in the database, no `CREATE EXTENSION` to run).

### Compiling

This module must be built using the standard PGXS infrastructure. For this to work, the `pg_config` program must be available in your $PATH:

```
git clone https://github.com/pierreforstmann/pgpasswd.git 
cd pgpasswd
make 
make install
```
`pgpasswd` is installed in the current PostgreSQL bin directory defined by the PGXS infrastructure.

## Usage

`pgpasswd` for Postgresql can be compared to `passwd` on Linux and UNIX systems.

Enter:
* same parameters (host name, port number, user name and database name) as psql to connect to PostgreSQL 
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
