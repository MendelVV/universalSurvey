#ifndef EXCELFUNCTION_H
#define EXCELFUNCTION_H

#include <ActiveQt\QAxBase.h>
#include <ActiveQt\QAxObject.h>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QDataStream>
#include <QVector>
#include "settingclass.h"
#include <QDate>
#include <time.h>
#include "excel_main/excelcell.h"

class ExcelFunction: public QObject
{
    Q_OBJECT
public:
    ExcelFunction(QString file, QString sheet);
    ExcelFunction(QString file, int num);
    ExcelFunction(QString file);
    ~ExcelFunction();

    void setSheet(int);

    void setVisible(bool b);
    void quit();
    void insertCells(QVector<ExcelCell*> vecCells);

protected:

//    QMap<QString, int> mapColumn;//словарь номеров ячеек. Ключ - тип ячейки (что в ней должно быть). значение - номер колонки
    QAxObject* excel;
    QAxObject *workbooks;
    QAxObject *workbook;
    QAxObject *mSheets;
    QAxObject *StatSheet;
signals:
};

#endif // EXCELFUNCTION_H


