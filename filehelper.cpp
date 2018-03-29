#include "filehelper.h"

FileHelper::FileHelper()
{

}

void FileHelper::makeDir(QString dir){
    dir = QCoreApplication::applicationDirPath()+"/"+dir;
    if (!QDir(dir).exists()){
        QDir().mkdir(dir);
    }
}

void FileHelper::runJar(){
    QString file = QCoreApplication::applicationDirPath()+"/charts.jar";
    QString dir = QCoreApplication::applicationDirPath()+"/charts/";
    //qDebug()<<dir;
    QProcess::execute("java", QStringList() << "-jar" << file <<"-dir "+dir);
}
