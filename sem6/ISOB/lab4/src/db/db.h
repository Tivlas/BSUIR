#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <qsqlquery.h>
#include <optional>

class DB
{
    static QSqlDatabase db;
public:
    static bool connect(QString sql_name, QString host_name, QString db_name, QString user_name, QString password);
    static std::optional<int> authenticate(QString login, QString password);
};
