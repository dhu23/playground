```
sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt $(lsb_release -cs)-pgdg main" > /etc/apt/sources.list.d/pgdg.list'
wget --quite -0 - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo -apt-key add -
sudo apt-get update
sudo apt-get install postgresql
sudo apt-get install postgresql-12
```


# Connect to the database server via psql
```
sudo -i -u postgres
curl -0 https://sp.postgresqltutorial.com/wp-content/uploads/2019/05/dvdrental.zip
unzip dvdrental.zip
psql
postgres=# create database dvdrental;
postgres=# \q

$ pg_restore --dbname=dvdrental --verbose dvdrental.tar

psql
postgres=# \c dvdrental

dvdrental=# select count(*) from film;

count 
-------
1000
(1 row)
```
