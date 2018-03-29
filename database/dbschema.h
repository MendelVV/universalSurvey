
#ifndef DBSCHEMA_H
#define DBSCHEMA_H

#include <QString>

class DbSchema
{
public:
    //все символы в нижнем регистре
    DbSchema();
    static const QString NAME;
    class TableSchoolsCatalog{
    public:
        static const QString NAME;

        class Cols{
        public:
            static const QString CODE;
            static const QString SCHOOLNAME;
            static const QString REGIONCODE;
        };
    };

};

#endif // DBSCHEMA_H
