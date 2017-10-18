#ifndef JSONHANDLE_H
#define JSONHANDLE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QVariantMap>
#include <QVariantList>
#include <QVariant>

#include <QObject>
#include <QDebug>
#include <QFile>

class JsonHandler : public QObject
{
    Q_OBJECT
public:
    static QVariantMap fromJson(QByteArray path);
    static void toJson(QByteArray path, QVariantMap *map);
};

#endif // JSONHANDLE_H
