#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QDir>
#include <QString>
#include <QFile>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
class FileHelper
{
public:
    FileHelper();
    static void makeDir(QString dir);
    static void runJar();
    static bool runTestJar();
    static void showResults();
};

#endif // FILEHELPER_H
