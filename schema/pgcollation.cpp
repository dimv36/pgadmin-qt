#include "schema/pgcollation.h"

PGCollation::PGCollation(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_COLLATION, name, QIcon(":/collation"))
{}

PGCollation::PGCollation(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_COLLATIONS, QObject::tr("Collations"), QIcon(":/collations")),
  _schemaOid(schemaOid)
{}

void PGCollation::appendOrRefreshObject(PGObject *object)
{
	PGCollation *collation = nullptr;

	QString query = "SELECT c.oid, c.collname, c.collcollate, c.collctype, \n"
					"       pg_get_userbyid(c.collowner) AS cowner, description \n"
					"FROM pg_collation c\n "
					"JOIN pg_namespace n ON n.oid = c.collnamespace \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = c.oid AND des.classoid = 'pg_collation'::regclass) \n"
					"WHERE c.collnamespace = %1::oid \n"
					"%2 \n"
					"ORDER BY c.collname";

	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND c.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString colname = set->value("collname");

			if (!object)
				collation = new PGCollation(_connection, colname);
			else
				collation = dynamic_cast<PGCollation *>(object);

			collation->setObjectAttribute("oid", set->oidValue("oid"));
			collation->setObjectAttribute("owner", set->value("cowner"));
			collation->setObjectAttribute("collate", set->value("collcollate"));
			collation->setObjectAttribute("ctype", set->value("collctype"));
			collation->setObjectAttribute("comment", set->value("description"));

			if (!object)
				addChild(collation);

			set->moveNext();
		}
		delete set;
	}
}

void PGCollation::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("LC_COLLATE"), _objectProperties.stringValue("collate"));
	tab->addRow(QObject::tr("LC_CTYPE"), _objectProperties.stringValue("ctype"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGCollation::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_collation WHERE oid = %1)")
										  .arg(oidString()));
}
