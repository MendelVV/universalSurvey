#include "jsonhelper.h"

JsonHelper::JsonHelper()
{

}

QJsonObject JsonHelper::readJson(QString fileName){

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
        qWarning()<<"no open file!";
        QJsonObject nothingObject;
        return nothingObject;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("CP1251"));

    QString data = stream.readAll();
    QJsonDocument loadDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = loadDoc.object();
    file.close();
    return jsonObject;
}
