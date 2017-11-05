#include "schema/pggroup.h"

PGGroup::PGGroup(const PGConnection *connection, const QString &name)
: PGRole(connection, OBJECT_GROUP, name, QIcon(":/group.png"))
{

}

PGGroup::PGGroup(PGConnection *connection)
: PGRole(connection, COLLECTION_GROUPS, QObject::tr("Group roles"), QIcon(":/groups.png"), QIcon(":/group.png"), false)
{
}

void PGGroup::refreshObjectProperties(PropertyTable *tab)
{
	PGRole::refreshObjectProperties(tab);
}
