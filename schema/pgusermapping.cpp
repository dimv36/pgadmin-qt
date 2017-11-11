#include "schema/pgusermapping.h"

PGUserMapping::PGUserMapping(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_USERMAPPING, name, QIcon(":/usermapping"))
{}

PGUserMapping::PGUserMapping(const PGConnection *connection, const Oid foreignServerOid)
: PGObject(connection, COLLECTION_USERMAPPINGS, QObject::tr("User Mappings"), QIcon(":/usermappings")),
  _foreignServerOid(foreignServerOid)
{}

void PGUserMapping::appendOrRefreshObject(PGObject *object)
{
	PGUserMapping *userMapping = nullptr;

	QString query = "SELECT u.oid AS um_oid, \n"
					"       CASE WHEN u.umuser = 0::oid THEN 'public'::name ELSE a.rolname END AS mapping_name, \n"
					"       array_to_string(u.umoptions, ', ') AS um_options \n"
					"FROM pg_user_mapping u\n "
					"LEFT JOIN pg_authid a ON a.oid = u.umuser\n"
					"WHERE u.umserver = %1 \n"
					"%2\n "
					"ORDER BY mapping_name";

	query = query.arg(QString::number(_foreignServerOid),
					  object ? QString("AND u.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString mappingName = set->value("mapping_name");

			if (!object)
				userMapping = new PGUserMapping(_connection, mappingName);
			else
				userMapping = dynamic_cast<PGUserMapping *>(object);

			userMapping->setObjectAttribute("oid", set->oidValue("um_oid"));
			userMapping->setObjectAttribute("options", set->value("um_options"));

			if (!object)
				addChild(userMapping);

			set->moveNext();
		}

		delete set;
	}
}

void PGUserMapping::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Options"), _objectProperties.stringValue("options"));
}

bool PGUserMapping::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_user_mapping WHERE oid = %1)")
										  .arg(oidString()));
}
