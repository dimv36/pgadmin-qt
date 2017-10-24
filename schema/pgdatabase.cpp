#include "schema/pgdatabase.h"

PGDatabase::PGDatabase(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_DATABASE, name, QIcon(":/database.png"))
{
	if (!(connection->databaseName() == name))
		setIcon(ColumnText, QIcon(":/database-disconnected.png"));
}

PGDatabase::PGDatabase(PGConnection *connection)
: PGObject(connection, COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"), QIcon(":/database.png"))
{
	_propertiesSQL = "SELECT dat.datname AS objname,\n"
					"        pg_get_userbyid(datdba) AS owner,\n"
					 "       des.description AS comment\n"
					 "FROM pg_database dat\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON dat.oid = des.objoid\n"
					 "ORDER by dat.oid;";
}

PGDatabase *PGDatabase::appendObject(const PGConnection *connection, const QString &name)
{
	return new PGDatabase(connection, name);
}

void PGDatabase::refreshObjectProperties(PropertyTable *tab)
{

}
