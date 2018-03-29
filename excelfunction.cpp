#include "excelfunction.h"

ExcelFunction::ExcelFunction(QString file, QString sheet)
{
    excel = new QAxObject("Excel.Application");
    workbooks = excel->querySubObject( "Workbooks" ); //получаем указатель на список книг
    workbook = workbooks->querySubObject( "Open(const QString&)", file );
    mSheets = workbook->querySubObject( "Sheets" );
    StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant(sheet));


/*    StatSheet->setProperty("AutoFilterMode",false);
    QAxObject* names = workbook->querySubObject("Names");
    int count = names->property("Count").toInt();
    qDebug()<<count;
    for (int i=0;i<count;i++){
        QAxObject* nm = workbook->querySubObject("Names(QVariant)",i+1);
        if (nm->property("Name").toString()=="_FilterDatabase"){
            QAxObject* filter = names->querySubObject("Item","_FilterDatabase");
            filter->dynamicCall("Delete");
        }
        delete nm;
    }
    names = StatSheet->querySubObject("Names");
    count = names->property("Count").toInt();
    qDebug()<<count;
    for (int i=0;i<count;i++){
        QAxObject* nm = StatSheet->querySubObject("Names(QVariant)",i+1);
        if (nm->property("Name").toString()=="_FilterDatabase"){
            QAxObject* filter = names->querySubObject("Item","_FilterDatabase");
            filter->dynamicCall("Delete");
        }
        delete nm;
    }*/
    excel->dynamicCall( "SetVisible(bool)", false);
}

ExcelFunction::ExcelFunction(QString file, int num)
{
    excel = new QAxObject("Excel.Application");
    workbooks = excel->querySubObject( "Workbooks" ); //получаем указатель на список книг
    excel->dynamicCall( "SetVisible(bool)", true);
    workbook = workbooks->querySubObject( "Open(const QString&)", file );
    mSheets = workbook->querySubObject( "Sheets" );
    StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant(num));

    excel->dynamicCall( "SetVisible(bool)", false);
}

ExcelFunction::ExcelFunction(QString file)
{
    excel = new QAxObject("Excel.Application");
    workbooks = excel->querySubObject( "Workbooks" ); //получаем указатель на список книг
    workbook = workbooks->querySubObject( "Open(const QString&)", file );
    mSheets = workbook->querySubObject( "Sheets" );
}

ExcelFunction::~ExcelFunction(){
    delete excel;
}

void ExcelFunction::setSheet(int num){
    StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant(num));
}


void ExcelFunction::setVisible(bool b){
    excel->dynamicCall( "SetVisible(bool)", b);
}

void ExcelFunction::quit(){
    workbook->dynamicCall("Close(QVariant)",false);
    excel->dynamicCall("Quit()");
}

void ExcelFunction::insertCells(QVector<ExcelCell *> vecCells){
    QAxObject* cell;

    foreach (ExcelCell* excelCell, vecCells) {
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", excelCell->getRow(), excelCell->getColumn());
        cell->setProperty("Value",excelCell->getValue());
        delete cell;
    }

}
