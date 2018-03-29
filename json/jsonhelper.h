#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

class JsonHelper
{
public:
    JsonHelper();
    static QJsonObject readJson(QString fileName);
};

#endif // JSONHELPER_H
