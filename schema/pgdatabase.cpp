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
					 "       des.description AS comment\n"
					 "FROM pg_database dat\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON dat.oid = des.objoid\n"
					 "ORDER by dat.oid;";
	int count = _connection->executeScalar("SELECT count(*) FROM pg_database").toInt();
	setText(ColumnText, QString("%1 (%2)").arg(text(ColumnText)).arg(QString::number(count)));
}

void PGDatabase::refreshObjectProperties(PropertyTable *tab)
{

}
