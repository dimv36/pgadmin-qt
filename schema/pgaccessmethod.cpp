#include "schema/pgaccessmethod.h"

PGAccessMethod::PGAccessMethod(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_ACCESSMETHOD, name, QIcon(":/access-method"))
{}

PGAccessMethod::PGAccessMethod(const PGConnection *connection)
	: PGObject(connection, COLLECTION_ACCESSMETHODS, QObject::tr("Access methods"), QIcon(":/access-methods"))
{}

bool PGAccessMethod::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_am WHERE oid = %1)")
										  .arg(_objectProperties.oidString()));
}

void PGAccessMethod::appendOrRefreshObject(PGObject *object)
{
	PGAccessMethod *accessMethod = nullptr;

	QString query = "SELECT am.oid AS oid, am.amname AS amname, am.amhandler AS amhandler,\n "
					"       CASE am.amtype WHEN 'i' THEN 'INDEX' ELSE NULL END AS amtype, d.description AS comment \n"
					"FROM pg_am am \n"
					"LEFT JOIN pg_description d ON am.oid = d.objoid\n AND d.classoid = 'pg_am'::regclass \n"
					"%1 \n"
					"ORDER BY am.amname";
	query = query.arg(object ? QString("WHERE am.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("amname");

			if (!object)
				accessMethod = new PGAccessMethod(_connection, name);
			else
				accessMethod = dynamic_cast<PGAccessMethod *>(object);

			accessMethod->setObjectAttribute("oid", set->oidValue("oid"));
			accessMethod->setObjectAttribute("handler", set->value("amhandler"));
			accessMethod->setObjectAttribute("amtype", set->value("amtype"));
			accessMethod->setObjectAttribute("comment", set->value("comment"));

			if (!object)
				addChild(accessMethod);

			set->moveNext();
		}
		delete set;
	}
}

void PGAccessMethod::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Handler"), _objectProperties.stringValue("handler"));
	tab->addRow(QObject::tr("Type"), _objectProperties.stringValue("amtype"));
	tab->addRow(QObject::tr("System access method?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}
