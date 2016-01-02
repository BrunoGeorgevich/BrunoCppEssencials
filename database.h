#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QFile>

class Database : public QObject
{

    Q_OBJECT

public:
    Database(QByteArray path);
    Database(QByteArray path, QByteArray userName, QByteArray password);
    bool dbExists();
    bool hasAnyTables();
    bool createTable(QByteArray tableName, QByteArray values);
    bool dropTable(QByteArray tableName);
    QVariantList select(QByteArray tableName, QByteArray criteria, QByteArray where);
    QVariantList select(QByteArray tableName, QByteArray criteria);
    bool deleteFrom(QByteArray tableName, QByteArray where);
    bool insertInto(QByteArray tableName, QByteArray values);
    bool insertInto(QByteArray tableName, QByteArray roles, QByteArray values);
    bool updateUser(QByteArray tableName, QByteArray updateValues, QByteArray where);
signals:
    void updateLog(QByteArray log);
private:
    QSqlDatabase db;
    QByteArray path;
};

#endif // DATABASE_H
