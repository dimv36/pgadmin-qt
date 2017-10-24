#include "schema/pguser.h"

PGUser::PGUser(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_USER, name, QIcon(":/user.png"))
{

}

PGUser::PGUser(PGConnection *connection)
: PGObject(connection, COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"), QIcon(":/user.png"))
{
	_propertiesSQL = "SELECT use.usename AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_user use\n"
					 "LEFT JOIN pg_description des\n"
					 "ON use.usesysid = des.objoid";
}

PGUser *PGUser::appendObject(const PGConnection *connection, const QString &name)
{
	return new PGUser(connection, name);
}

void PGUser::refreshObjectProperties(PropertyTable *)
{

}
