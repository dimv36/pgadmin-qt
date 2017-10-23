#include "schema/pggroup.h"

PGGroup::PGGroup(const QString &name)
: PGObject(OBJECT_GROUP, name, QIcon(":/group.png"))
{

}

PGGroup::PGGroup(PGConnection *connection)
	: PGObject(COLLECTION_GROUPS, QObject::tr("Group roles"), QIcon(":/groups.png"), QIcon(":/group.png"))
{
	setConnection(connection);
	_propertiesSQL = "SELECT gro.groname AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_group gro\n"
					 "LEFT JOIN pg_description des\n"
					 "ON gro.grosysid = des.objoid;";
}

void PGGroup::refreshObjectProperties(PropertyTable *)
{

}

PGGroup *PGGroup::appendObject(const QString &name)
{
	return new PGGroup(name);
}
