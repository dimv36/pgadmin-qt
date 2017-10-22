#include "pgdatabase.h"

PGDatabase::PGDatabase(const QString &name)
: PGObject(OBJECT_DATABASE, name, QIcon(":/database.png"))
{

}

PGDatabase::PGDatabase()
: PGObject(COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"))
{

}

void PGDatabase::refreshProperties(PropertyTable *)
{

}
