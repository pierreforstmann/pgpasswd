# pgpasswd
Change PostgreSQL account password from Linux command line without entering any SQL statement.<br>

`pgpasswd`  has been successfully tested with PostgreSQL 9.5, 9.6, 10, 11, 12 and 13 on Linux. <br>

## usage

Enter:
* same parameters (host name, port number, user name and database name) as psql to connect to PostgreSQL 
* current password 
* new password twice.

Example:

``` 
pgpasswd -h localhost -p 5432 -U test -d postgres
Password:
New password:
New password:
Password changed.
```
