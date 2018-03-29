#include "dbhelper.h"

DbHelper::DbHelper()
{

}

QVector<QVector<QString> > DbHelper::getSchoolsList(){
    QString path=QCoreApplication::applicationDirPath()+"/";
    SQLiteClass db(path+DbSchema::NAME,"survey");
    QVector<QString> cols;
    cols<<DbSchema::TableSchoolsCatalog::Cols::CODE<<DbSchema::TableSchoolsCatalog::Cols::SCHOOLNAME;
    QVector<QVector<QString> > vecData = db.select(DbSchema::TableSchoolsCatalog::NAME,cols,"",cols);
    return vecData;
}
