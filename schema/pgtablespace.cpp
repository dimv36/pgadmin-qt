#include "schema/pgtablespace.h"

PGTablespace::PGTablespace(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TABLESPACE, name, QIcon(":/tablespace.png"))
{

}

PGTablespace::PGTablespace(PGConnection *connection)
: PGObject(connection, COLLECTION_TABLESPACES, QObject::tr("Tablespaces"), QIcon(":/tablespaces.png"), QIcon(":/tablespace.png"))
{
	_propertiesSQL = "SELECT spc.spcname AS objname,\n"
					 "       pg_get_userbyid(spc.spcowner) AS owner,\n"
					 "       des.description AS comment\n"
					 "FROM pg_tablespace spc\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON spc.oid = des.objoid\n"
					 "ORDER BY spc.oid;";
}

void PGTablespace::refreshObjectProperties(PropertyTable *)
{

}

PGTablespace* PGTablespace::appendObject(const PGConnection *connection, const QString &name)
{
	return new PGTablespace(connection, name);
}
