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
    qDebug()<<"dir = "<<dir;
    QProcess::execute("java", QStringList() << "-jar" << file <<"-dir "+dir);
}

bool FileHelper::runTestJar(){
    QString fileName = QCoreApplication::applicationDirPath()+"/charts.jar";
    QString dir = QCoreApplication::applicationDirPath()+"/";
    QProcess::execute("java", QStringList() << "-jar" << fileName <<"-dir "+dir<<"-test");
    QProcess::execute("java", QStringList() << "-jar" << fileName <<"-dir "+dir<<"-test");
    QFile file(dir+"test.txt");
    bool b = file.exists();
    if (b){
        file.remove();
    }
    return b;
}

void FileHelper::showResults(){
    QString dir = QCoreApplication::applicationDirPath()+"/results/";
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
}
