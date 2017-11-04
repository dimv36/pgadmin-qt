#include "schema/pgtablespace.h"

PGTablespace::PGTablespace(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TABLESPACE, name, QIcon(":/tablespace.png"))
{

}

PGTablespace::PGTablespace(PGConnection *connection)
: PGObject(connection, COLLECTION_TABLESPACES, QObject::tr("Tablespaces"), QIcon(":/tablespaces.png"), QIcon(":/tablespace.png"))
{
//	_propertiesSQL = "SELECT spc.oid AS oid, spc.spcname AS objname,\n"
//					 "       pg_get_userbyid(spc.spcowner) AS owner,\n"
//					 "       des.description AS comment\n"
//					 "FROM pg_tablespace spc\n"
//					 "LEFT JOIN pg_shdescription des\n"
//					 "ON spc.oid = des.objoid\n"
//					 "ORDER BY spc.oid;";
}

void PGTablespace::refreshObjectProperties(PropertyTable *tab)
{
//	tab->setHeaders();

//	QString query = "SELECT ts.oid, spcname, pg_catalog.pg_tablespace_location(ts.oid) AS spclocation, spcoptions, "
//					"       pg_get_userbyid(spcowner) as spcuser, spcacl, "
//					"       pg_catalog.shobj_description(oid, 'pg_tablespace') AS description, "
//					"       (SELECT array_agg(label) FROM pg_shseclabel sl1 WHERE sl1.objoid=ts.oid) AS labels, "
//					"       (SELECT array_agg(provider) FROM pg_shseclabel sl2 WHERE sl2.objoid=ts.oid) AS providers "
//					"       FROM pg_tablespace ts\n"
//					"WHERE ts.oid = " + QString::number(_oid);

//	PGSet *set = _connection->executeSet(query);

//	if (set)
//	{
//		tab->addRow(QObject::tr("Name"), _name);
//		tab->addRow(QObject::tr("OID"), QString::number(_oid));
//		tab->addRow(QObject::tr("Owner"), set->value("spcuser"));
//		tab->addRow(QObject::tr("Location"), set->value("spclocation"));
//		tab->addRow(QObject::tr("ACL"), set->value("spcacl"));
//		tab->addRow(QObject::tr("Comment"), set->value("description"));
//		delete set;
//	}
}
