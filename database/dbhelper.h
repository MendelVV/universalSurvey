#ifndef DBHELPER_H
#define DBHELPER_H

#include "database/dbschema.h"
#include "excel_database/sqliteclass.h"

class DbHelper
{
public:
    DbHelper();
    static QVector<QVector<QString> > getSchoolsList();
protected:
    static SQLiteCursor* getSchoolCursor();
};

#endif // DBHELPER_H
