#include "schema/pgdatabase.h"

PGDatabase::PGDatabase(const QString &name)
: PGObject(OBJECT_DATABASE, name, QIcon(":/database.png"))
{

}

PGDatabase::PGDatabase(PGConnection *connection)
: PGObject(COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"), QIcon(":/database.png"))
{
	setConnection(connection);
	_propertiesSQL = "SELECT dat.datname AS objname,\n"
					"        pg_get_userbyid(datdba) AS owner,\n"
					 "       des.description AS comment\n"
					 "FROM pg_database dat\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON dat.oid = des.objoid\n"
					 "ORDER by dat.oid;";
}

PGDatabase *PGDatabase::appendObject(const QString &name)
{
	return new PGDatabase(name);
}

void PGDatabase::refreshObjectProperties(PropertyTable *tab)
{

}
