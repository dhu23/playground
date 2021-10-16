# step1 -- installing PostgreSQL

```
$ sudo apt update
```

Then install the package along with a -contrib package that adds some
additional utilities and functionality:

```
$ sudo apt install postgresql postgresql-contrib
```


# step 2 -- using postgresSQL roles and databases
Upon installation, Postgres is set up to use *ident* authentication, 
meaning that it associates Postgres roles with a matching Unix/Linux 
system account. If a role exists within Postgres, a Unix/Linux username
with the same name is able to sign in as that role.

The installation procedure created a user account called *postgres* that
is associated with the default Postgres role. 

One way to switch over to the *postgres* account on your server by typing
```
$ sudo -i -u postgres
```

Then you can access the Postgres prompt by typing
```
$ psql
```

To exit out of the prompt, type
```
postgres=# \q
```

This brings you back to the postgres Linux command prompt. To return to
your regular system user, run `exit` command.
```
postgres@server:~$ exit
```

Another way is the run the following:
```
$ sudo -u postgres psql
```

The will log you directly into Postgres without the intermediary `bash` shell
Again you can exit the interactive session by typing `\q`


# step 3 -- creating a new role

```
postgres@server:~$ creatuser --interactive   ### create a new role
```

Or
```
$ sudo -u postgres createuser --interactive
```


# step 4 -- creating a new database
Another assumption that the postgres authentiation system makes by default
is that for any role used to log in, that role will have a database with the
same name which it can access. This means that if the user you created in 
the last section is called *sammy*, that role will attempt to connect to 
a database which is also called "sammy". You can create the appropriate
database with the `createdb` command

```
postgres@server:~$ createdb sammy
```

If instead you prefer to use `sudo` for each command, you can try
```
$ sudo -u postgres createdb sammy
```


# step 5 -- opening a Postgres prompt with the new role
To log in with `ident` based authentication, you'll need a Linux user with 
the sname name as your Postgres role and database. If you don't have a 
matching Linux user available, you can create one with the `adduser` command.
You will have to do this from your non-root account with `sudo`. 

```
$ sudo adduser sammy
```

Once this new account is available, you can either switch over and connect
to the database by typing:

```
$ sudo -i -u sammy
$ psql
```

Or you can do this inline:
```
$ sudo -u sammy psql
```
