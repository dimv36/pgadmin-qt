#include "schema/pguser.h"

PGUser::PGUser(const PGConnection *connection, const QString &name)
: PGRole(connection, OBJECT_USER, name, QIcon(":/user.png"))
{

}

PGUser::PGUser(PGConnection *connection)
: PGRole(connection, COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"), QIcon(":/user.png"), true)
{
}

void PGUser::refreshObjectProperties(PropertyTable *tab)
{
	PGRole::refreshObjectProperties(tab);
}
