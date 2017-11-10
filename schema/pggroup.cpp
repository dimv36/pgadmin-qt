#include "schema/pggroup.h"

PGGroup::PGGroup(const PGConnection *connection, const QString &name)
: PGRole(connection, OBJECT_GROUP, name, QIcon(":/group"))
{}

PGGroup::PGGroup(const PGConnection *connection)
: PGRole(connection, COLLECTION_GROUPS, QObject::tr("Group roles"), QIcon(":/group"), false)
{}
