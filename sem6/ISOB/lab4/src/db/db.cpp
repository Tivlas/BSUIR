#include "db.h"

QSqlDatabase DB::db;

bool DB::connect(QString sql_name, QString host_name, QString db_name, QString user_name, QString password) {
    DB::db = QSqlDatabase::addDatabase(sql_name);
    DB::db.setHostName(host_name);
    DB::db.setDatabaseName(db_name);
    DB::db.setUserName(user_name);
    DB::db.setPassword(password);
    return DB::db.open();
}

std::optional<int> DB::authenticate(QString login, QString password) {
    QSqlQuery query;
    query.prepare("select role_id from users where username=:login and password=:password;");
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    //query.exec("select role_id from users where username='" + login + "' and password='" + password + "';");
    query.exec();

    if (query.first()) {
        size_t role_id = query.value(0).toInt();
        query.prepare("select privilege_lvl from roles where role_id=:id;");
        query.bindValue(":id", role_id);
        query.exec();
        if (query.first()) {
            return {query.value(0).toInt()};
        } else {
            return {};
        }
    } else {
        return {};
    }
}
