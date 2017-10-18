#include "jsonhandler.h"

QVariantMap JsonHandler::fromJson(QByteArray path)
{
    QVariantMap map;

    QFile f(path);
    if(!f.exists()) {
        qDebug() << "FILE DOESN'T EXISTS!";
        return map;
    }

    if(!f.open(QIODevice::ReadOnly)) {
        qDebug() << "FILE DOESN'T OPENS!";
        return map;
    }

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    QJsonObject primaryObj = doc.object();

    f.close();

    foreach (QString k, primaryObj.keys()) {
        QJsonValue val = primaryObj[k];
        if(val.isString()) map.insert(k,val.toString());
        else if(val.isDouble()) map.insert(k,val.toDouble());
        else if(val.isObject()) map.insert(k,val.toObject());
        else if(val.isBool()) map.insert(k,val.toBool());
        else if(val.isArray()) map.insert(k,val.toArray());
        else map.insert(k,QVariant());
    }

    return map;
}

void JsonHandler::toJson(QByteArray path, QVariantMap *map)
{
    QJsonObject primaryObj;
    foreach (QString k, map->keys()) {
        QVariant val = map->value(k);
        if(val.canConvert<QString>()) primaryObj.insert(k,val.toString());
        else if(val.canConvert<int>()) primaryObj.insert(k,val.toInt());
        else if(val.canConvert<double>()) primaryObj.insert(k,val.toDouble());
        else if(val.canConvert<QJsonObject>()) primaryObj.insert(k,val.toJsonObject());
        else if(val.canConvert<bool>()) primaryObj.insert(k,val.toBool());
        else if(val.canConvert<QJsonArray>()) primaryObj.insert(k,val.toJsonArray());
        else primaryObj.insert(k,QJsonValue());
    }

    QJsonDocument doc(primaryObj);
    QByteArray data = doc.toJson();

    QFile f(path);
    if(f.exists()) {
        qDebug() << "FILE ALREADY EXISTS!";
    }

    if(!f.open(QIODevice::WriteOnly)) {
        qDebug() << "FILE DOESN'T OPENS!";
        return;
    }

    if(f.write(data) < 0) {
        qDebug() << "OCCURRED AN ERROR IN THE WRITING";
        return;
    }

    qDebug() << "FILE CREATED WITH SUCCESS";
    f.close();
}
