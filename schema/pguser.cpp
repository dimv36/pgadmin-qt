#include "schema/pguser.h"

PGUser::PGUser(const PGConnection *connection, const QString &name)
: PGRole(connection, OBJECT_USER, name, QIcon(":/user"))
{}

PGUser::PGUser(const PGConnection *connection)
: PGRole(connection, COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/user"), true)
{}
