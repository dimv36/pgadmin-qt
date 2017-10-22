#include "pguser.h"

PGUser::PGUser(PGConnection *connection)
: PGObject(COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"), QIcon(":/user.png"))
{
	setConnection(connection);
	int count = _connection->executeScalar("SELECT count(*) FROM pg_user").toInt();
	setText(ColumnText, QString("%1 (%2)").arg(text(ColumnText)).arg(QString::number(count)));
	_propertiesSQL = "SELECT use.usename AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_user use\n"
					 "LEFT JOIN pg_description des\n"
					 "ON use.usesysid = des.objoid";
}

void PGUser::refreshObjectProperties(PropertyTable *)
{

}
