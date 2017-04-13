#include "database.h"

Database::Database(QByteArray path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.open();
    if(!db.isOpen())
        emit updateLog("ERR.01 : DB CANNOT BE OPENED!");
    else
        emit updateLog("DB OPENED WITH SUCESS!");
    this->path = path;
}

Database::Database(QByteArray path, QByteArray userName, QByteArray password)
{
    db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
    db.setDatabaseName(path);
    db.setUserName(userName);
    db.setPassword(password);
    db.open();
    if(!db.isOpen())
        emit updateLog("ERR.01 : DB CANNOT BE OPENED!");
    else
        emit updateLog("DB OPENED WITH SUCESS!");
    this->path = path;
}

bool Database::dbExists()
{
    QFile dbFile(path);
    return dbFile.exists();
}

bool Database::hasAnyTables()
{
    if(db.tables().length())
        return true;
    return false;
}

QStringList Database::tables()
{
    return db.tables();
}

bool Database::createTable(QByteArray tableName, QList<QByteArray> values)
{
    QSqlQuery query(db);
    QByteArray aux = "";
    foreach (QByteArray v, values) {
        aux += v + ",";
    }
    aux.remove(aux.size() - 1,1);
    bool ok = query.exec("CREATE TABLE "
                         +tableName+
                         "("+aux+
                         ")");
    if(!ok)
        qDebug() << query.lastError().text();
    else
        qDebug() << "TABLE CREATED WITH SUCESS!";

    return ok;
}

bool Database::dropTable(QByteArray tableName)
{
    QSqlQuery query(db);
    bool ok = query.exec("DROP TABLE " + tableName);

    if(!ok)
        emit updateLog(("ERR.02 : " + query.lastError().text()).toLatin1());
    else
        emit updateLog("TABLE DROPPED WITH SUCESS!");

    return ok;
}

QVariantList Database::select(QByteArray tableName, QByteArray criteria, QByteArray where)
{
    QSqlQuery query(db);
    bool ok = query.exec("SELECT " + criteria + " FROM " + tableName + " WHERE " + where);

    QVariantList list;

    if(!ok) {
        emit updateLog(("ERR.03 : " + query.lastError().text()).toLatin1());
        return list;
    }

    while(query.next()) {
        QVariantMap map;
        QSqlRecord rec = query.record();
        for(int i=0;i<rec.count();i++) {
            map.insert(rec.fieldName(i),query.value(rec.fieldName(i)));
        }
        list.append(map);
    }
    emit updateLog("SELECT COMMAND EXECUTED WITH SUCESS!");
    return list;
}

QVariantList Database::select(QByteArray tableName, QByteArray criteria)
{
    QSqlQuery query(db);
    bool ok = query.exec("SELECT " + criteria + " FROM " + tableName);

    QVariantList list;

    if(!ok) {
        emit updateLog(("ERR.03 : " + query.lastError().text()).toLatin1());
        return list;
    }

    while(query.next()) {
        QVariantMap map;
        QSqlRecord rec = query.record();
        for(int i=0;i<rec.count();i++) {
            map.insert(rec.fieldName(i),query.value(rec.fieldName(i)));
        }
        list.append(map);
    }

    emit updateLog("SELECT COMMAND EXECUTED WITH SUCESS!");
    return list;
}

bool Database::deleteFrom(QByteArray tableName, QByteArray where)
{
    QSqlQuery query(db);

    QVariantList list = select(tableName,"*",where);
    bool ok;

    if(list.isEmpty())
        ok = false;
    else
        ok = true;

    if(ok) {
        query.exec("DELETE FROM " + tableName + " WHERE " + where);
        emit updateLog("DELETE COMMAND EXECUTED WITH SUCESS!");
    } else {
        emit updateLog("ERR.04 : WHERE CLUASULE DOESN'T FOUND ANYTHING!");
    }

    return ok;
}

bool Database::insertInto(QByteArray tableName, QMap<QString, QString> properties)
{
    QSqlQuery query(db);
    QByteArray roles = "";
    QByteArray values = "";

    foreach (QString k, properties.keys()) {
        roles += k + ",";
        values += properties.value(k) + ",";
    }

    roles.remove(roles.size() - 1,1);
    values.remove(values.size() - 1,1);

    bool ok = query.exec("INSERT INTO "+
                         tableName+
                         "("+
                         roles+
                         ")"+
                         " VALUES("+
                         values+
                         ")");

    if(!ok) {
        emit updateLog(("ERR.05 : " + query.lastError().text()).toLatin1());
        qDebug() << "INSERT INTO "+
                    tableName+
                    "("+
                    roles+
                    ")"+
                    " VALUES("+
                    values+
                    ")";
    }
    else
        emit updateLog("INSERT COMMAND EXECUTED WITH SUCESS!");

    return ok;
}

bool Database::updateUser(QByteArray tableName, QMap<QString, QString> properties, QByteArray where)
{
    QSqlQuery query(db);
    QByteArray updateValues = "";
    foreach (QString k, properties.keys()) {
        updateValues += k+"="+properties.value(k)+",";
    }
    updateValues.remove(updateValues.size()-1,1);
    bool ok = query.exec("UPDATE "+
                         tableName+
                         " SET "+
                         updateValues+
                         " WHERE "+
                         where);

    if(!ok)
        emit updateLog(("ERR.06 : " + query.lastError().text()).toLatin1());
    else
        emit updateLog("UPDATE COMMAND EXECUTED WITH SUCESS!");

    return ok;
}
