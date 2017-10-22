#include "schema/pggroup.h"

PGGroup::PGGroup(PGConnection *connection)
	: PGObject(COLLECTION_GROUPS, QObject::tr("Group roles"), QIcon(":/groups.png"), QIcon(":/group.png"))
{
	setConnection(connection);
	int count = _connection->executeScalar("SELECT count(*) FROM pg_group").toInt();
	setText(ColumnText, QString("%1 (%2)").arg(text(ColumnText)).arg(QString::number(count)));
	_propertiesSQL = "SELECT gro.groname AS objname,\n"
					 "       des.description AS comment\n"
					 "FROM pg_group gro\n"
					 "LEFT JOIN pg_description des\n"
					 "ON gro.grosysid = des.objoid;";
}

void PGGroup::refreshObjectProperties(PropertyTable *)
{

}
