#include "pgtablespace.h"

PGTablespace::PGTablespace(PGConnection *connection)
: PGObject(COLLECTION_TABLESPACES, QObject::tr("Tablespaces"), QIcon(":/tablespaces.png"), QIcon(":/tablespace.png"))
{
	setConnection(connection);
	int count = _connection->executeScalar("SELECT count(*) FROM pg_tablespace").toInt();
	setText(ColumnText, QString("%1 (%2)").arg(text(ColumnText)).arg(QString::number(count)));
	_propertiesSQL = "SELECT spc.spcname AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_tablespace spc\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON spc.oid = des.objoid\n"
					 "ORDER BY spc.oid;";
}

void PGTablespace::refreshObjectProperties(PropertyTable *)
{

}
