#include "schema/pgtablespace.h"

PGTablespace::PGTablespace(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TABLESPACE, name, QIcon(":/tablespace.png"))
{}

PGTablespace::PGTablespace(const PGConnection *connection)
: PGObject(connection, COLLECTION_TABLESPACES, QObject::tr("Tablespaces"), QIcon(":/tablespaces.png"), QIcon(":/tablespace.png"))
{}

void PGTablespace::appendOrRefreshObject(PGObject *object)
{
	PGTablespace *tablespace = nullptr;

	QString query = "SELECT ts.oid AS oid, spcname, pg_catalog.pg_tablespace_location(ts.oid) AS spclocation, spcoptions, \n"
					"       pg_get_userbyid(spcowner) AS owner, spcacl, \n"
					"       pg_catalog.shobj_description(oid, 'pg_tablespace') AS comment, \n"
					"       (SELECT array_agg(label) FROM pg_shseclabel sl1 WHERE sl1.objoid=ts.oid) AS labels, \n"
					"       (SELECT array_agg(provider) FROM pg_shseclabel sl2 WHERE sl2.objoid=ts.oid) AS providers \n"
					"       FROM pg_tablespace ts \n";
	if (object)
		query += QString("WHERE ts.oid = %1").arg(object->oidObjectAttribute("oid"));
	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString spcname = set->value("spcname");

			if (!object)
				tablespace = new PGTablespace(_connection, spcname);
			else
				tablespace = dynamic_cast<PGTablespace *> (object);
			tablespace->setObjectAttribute("oid", set->oidValue("oid"));
			tablespace->setObjectAttribute("owner", set->value("owner"));
			tablespace->setObjectAttribute("location", set->value("spclocation"));
			tablespace->setObjectAttribute("acl", set->value("spcacl"));
			tablespace->setObjectAttribute("comment", set->value("comment"));
			tablespace->parseSecurityLabels(set->value("providers"), set->value("labels"));

			if (!object)
				addChild(tablespace);
			set->moveNext();
		}
		delete set;
	}
}

void PGTablespace::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Location"), _objectProperties.stringValue("location"));
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	appendSecurityLabels(tab);
}
