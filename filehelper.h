#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QDir>
#include <QString>
#include <QFile>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

class FileHelper
{
public:
    FileHelper();
    static void makeDir(QString dir);
    static void runJar();
};

#endif // FILEHELPER_H
