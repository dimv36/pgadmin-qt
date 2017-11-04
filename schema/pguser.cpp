#include "schema/pguser.h"

PGUser::PGUser(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_USER, name, QIcon(":/user.png"))
{

}

PGUser::PGUser(PGConnection *connection)
: PGObject(connection, COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"), QIcon(":/user.png"))
{
//	_propertiesSQL = "SELECT use.usesysid AS oid, use.usename AS objname,\n"
//					 "       des.description AS comment\n"
//					 "FROM pg_user use\n"
//					 "LEFT JOIN pg_description des\n"
//					 "ON use.usesysid = des.objoid";
}

void PGUser::refreshObjectProperties(PropertyTable *tab)
{
	tab->setHeaders();
	
	
}
