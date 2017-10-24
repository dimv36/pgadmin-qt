#include "schema/pggroup.h"

PGGroup::PGGroup(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_GROUP, name, QIcon(":/group.png"))
{

}

PGGroup::PGGroup(PGConnection *connection)
: PGObject(connection, COLLECTION_GROUPS, QObject::tr("Group roles"), QIcon(":/groups.png"), QIcon(":/group.png"))
{
	_propertiesSQL = "SELECT gro.groname AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_group gro\n"
					 "LEFT JOIN pg_description des\n"
					 "ON gro.grosysid = des.objoid;";
}

void PGGroup::refreshObjectProperties(PropertyTable *)
{

}

PGGroup *PGGroup::appendObject(const PGConnection *connection, const QString &name)
{
	return new PGGroup(connection, name);
}
