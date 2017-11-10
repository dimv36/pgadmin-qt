#include "include/schema/pgextension.h"

PGExtension::PGExtension(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_EXTENSION, name, QIcon(":/extension"))
{}

PGExtension::PGExtension(const PGConnection *connection)
: PGObject(connection, COLLECTION_EXTENSIONS, QObject::tr("Extensions"), QIcon(":/extensions"))
{}

bool PGExtension::exists() const
{
	return true;
}

void PGExtension::appendOrRefreshObject(PGObject *object)
{
	PGExtension *extension = nullptr;

	QString query ="SELECT x.oid AS oid, pg_get_userbyid(extowner) AS owner, x.extname, n.nspname, \n"
				   "       x.extrelocatable, x.extversion, e.comment\n "
				   "FROM pg_extension x\n "
				   "JOIN pg_namespace n on x.extnamespace = n.oid\n "
				   "JOIN pg_available_extensions() e(name, default_version, comment) ON x.extname = e.name\n "
				   "%1\n "
				   "ORDER BY x.extname";
	query = query.arg(object ? QString("WHERE x.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("extname");

			if (!object)
				extension = new PGExtension(_connection, name);
			else
				extension = dynamic_cast<PGExtension *>(object);

			extension->setObjectAttribute("oid", set->oidValue("oid"));
			extension->setObjectAttribute("owner", set->value("owner"));
			extension->setObjectAttribute("schema", set->value("nspname"));
			extension->setObjectAttribute("relocatable", set->boolValue("extrelocatable"));
			extension->setObjectAttribute("version", set->value("extversion"));
			extension->setObjectAttribute("comment", set->value("comment"));

			if (!object)
				addChild(extension);

			set->moveNext();
		}
		delete set;
	}
}

void PGExtension::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Schema"), _objectProperties.stringValue("schema"));
	tab->addRow(QObject::tr("Relocatable?"), _objectProperties.boolValue("relocatable"));
	tab->addRow(QObject::tr("Version"), _objectProperties.stringValue("version"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}
