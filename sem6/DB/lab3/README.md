У некоторых возникает проблема с запуском лабы. Как исправить:
нужно создать двух дефолтных юзеров (можно через терминал (далее команды будут), я так делал, можно через sql developer):

*sqlplus '/ as sysdba'* (**это только для терминала, но войти надо от sysdba** и в девелопере) <br />
*alter session set "_ORACLE_SCRIPT"=true;* <br />
create user **USERNAME(для лабы создайте __dev__ и __prod__)** identified by **"password"**;  <br />

Затем создайте точно так же еще одно юзера (допустим LABUSER), запомните имя и пароль, в конце выдайте ему права:<br />
*grant sysdba to LABUSER container=all;*<br />

Потом создайте connection к базе от имени LABUSER, при создании указываете роль (по умолчанию стоит **default**) *sysdba*. <br />

Создайте все таблицы, функции и процедуры.
Теперь должно работать.