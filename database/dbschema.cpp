#include "dbschema.h"

DbSchema::DbSchema()
{

}

const QString DbSchema::NAME  = "setting.sqlite";

const QString DbSchema::TableSchoolsCatalog::NAME  = "tableschoolscatalog";
const QString DbSchema::TableSchoolsCatalog::Cols::CODE  = "code";
const QString DbSchema::TableSchoolsCatalog::Cols::SCHOOLNAME  = "schoolname";
const QString DbSchema::TableSchoolsCatalog::Cols::REGIONCODE  = "regioncode";
