#include "schema/pguser.h"

PGUser::PGUser(const QString &name)
: PGObject(OBJECT_USER, name, QIcon(":/user.png"))
{

}

PGUser::PGUser(PGConnection *connection)
: PGObject(COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"), QIcon(":/user.png"))
{
	setConnection(connection);
	_propertiesSQL = "SELECT use.usename AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_user use\n"
					 "LEFT JOIN pg_description des\n"
					 "ON use.usesysid = des.objoid";
}

PGUser *PGUser::appendObject(const QString &name)
{
	return new PGUser(name);
}

void PGUser::refreshObjectProperties(PropertyTable *)
{

}
