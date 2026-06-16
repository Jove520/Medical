#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDefaultDatabase();
    bool initializeSchema();

    QString databasePath() const;
    QString lastError() const;
    QSqlDatabase database() const;

private:
    DatabaseManager() = default;

    bool executeSql(const QString& sql);

private:
    QSqlDatabase m_database;
    QString m_databasePath;
    QString m_lastError;
};

#endif