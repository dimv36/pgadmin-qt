#include "schema/pgrole.h"
#include "schema/pggroup.h"
#include "schema/pguser.h"

PGRole::PGRole(const PGConnection *connection, const ObjectType objectType, const QString &name, const QIcon &icon)
: PGObject(connection, objectType, name, icon)
{}

PGRole::PGRole(PGConnection *connection, const ObjectType objectType, const QString &name, const QIcon &collectionIcon, const QIcon &objectIcon, bool canLogin)
: PGObject(connection, objectType, name, collectionIcon, objectIcon),
  _canlogin(canLogin)
{
	QString table;
	if (_connection->hasPrivilege("TABLE", "pg_authid", "SELECT"))
		table = "pg_authid";
	else
		table = "pg_roles";

	QString query = "SELECT tab.oid, tab.*, pg_catalog.shobj_description(tab.oid, 'pg_authid') AS comment, \n"
					"(SELECT array_agg(label) FROM pg_shseclabel sl1 WHERE sl1.objoid = tab.oid) AS labels, \n"
					"(SELECT array_agg(provider) FROM pg_shseclabel sl2 WHERE sl2.objoid = tab.oid) AS providers\n "
					"FROM %1 tab\n "
					"WHERE rolcanlogin %2\n "
					"ORDER BY rolname";
	query = query.arg(table).arg(_canlogin ? "IS TRUE" : "IS FALSE");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString rolename = set->value("rolname");
			PGRole *role = nullptr;

			if (_objtype == COLLECTION_GROUPS)
				role = new PGGroup(_connection, rolename);
			else if (_objtype == COLLECTION_USERS)
				role = new PGUser(_connection, rolename);

			role->setObjectAttribute("oid", set->oidValue("oid"));
			role->setObjectAttribute("canlogin", set->boolValue("rolcanlogin"));
			role->setObjectAttribute("inherit", set->boolValue("rolinherit"));
			role->setObjectAttribute("createrole", set->boolValue("rolcreaterole"));
			role->setObjectAttribute("createdb", set->boolValue("rolcreatedb"));
			role->setObjectAttribute("superuser", set->boolValue("rolsuper"));
			if (!_connection->backendVersionGE(9, 5))
				role->setObjectAttribute("catcanupdate", set->boolValue("rolcatupdate"));
			role->setObjectAttribute("validuntil", set->datetimeValue("rolvaliduntil"));
			role->setObjectAttribute("validinfinity", set->value("rolvaliduntil") == "infinity" ? true : false);
			role->setObjectAttribute("password", set->value("rolpassword"));
			role->setObjectAttribute("comment", set->value("comment"));
			role->setObjectAttribute("connlimit", set->intValue("rolconnlimit"));
			role->setObjectAttribute("replication", set->value("rolreplication"));
			parseSecurityLabels(set->value("providers"), set->value("labels"));

			// Get members of current role
			QString roleQuery = QString("WITH RECURSIVE cte "
										"AS \n"
										"( \n"
										"  SELECT oid FROM pg_roles WHERE oid = %1 \n"
										"  UNION ALL \n"
										"  SELECT m.roleid \n"
										"  FROM cte \n"
										"  JOIN pg_auth_members m ON m.member = cte.oid \n"
										") \n"
										"SELECT array_agg(pg_get_userbyid(oid)) FROM cte \n"
										"WHERE oid <> %1").arg(QString::number(role->oidObjectAttribute("oid")));
			QString members = _connection->executeScalar(roleQuery);
			members = members.remove('{').remove('}').replace(',', ", ");
			role->setObjectAttribute("members", members);

			// Get configuration
			role->getConfigurationVariables();

			addChild(role);
			set->moveNext();
		}
		refreshCollectionTitle(set->rowsCount());
		delete set;
	}
}

void PGRole::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Account expires"), _objectProperties.dateTimeStringValue("validuntil"));
	tab->addRow(QObject::tr("Can login?"), _objectProperties.boolValue("canlogin"));
	tab->addRow(QObject::tr("Superuser?"), _objectProperties.boolValue("superuser"));
	tab->addRow(QObject::tr("Create databases?"), _objectProperties.boolValue("createdb"));
	tab->addRow(QObject::tr("Create roles?"), _objectProperties.boolValue("createrole"));
	if (!_connection->backendVersionGE(9, 5))
		tab->addRow(QObject::tr("Update catalogs?"), _objectProperties.boolValue("catcanupdate"));
	tab->addRow(QObject::tr("Inherits?"), _objectProperties.boolValue("inherit"));
	tab->addRow(QObject::tr("Replication?"), _objectProperties.boolValue("replication"));
	tab->addRow(QObject::tr("Connection limit"), _objectProperties.intValue("connlimit"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	tab->addRow(QObject::tr("Members of"), _objectProperties.stringValue("members"));
	// Append settings
	PGKeyValueSettings settings = _objectProperties.variables();
	for (auto it = settings.begin(); it != settings.end(); ++it)
	{
		PGKeyValueSetting setting = *it;
		tab->addRow(setting.first, setting.second, QIcon(":/variable.png"));
	}
	appendSecurityLabels(tab);
}

void PGRole::getConfigurationVariables()
{
	PGKeyValueSettings settingsArray;

	QString query = "WITH configs\n"
					"AS\n "
					"(\n"
					"    SELECT datname, unnest(setconfig) AS config\n "
					"    FROM pg_db_role_setting s\n "
					"    LEFT JOIN pg_database d ON d.oid = s.setdatabase\n "
					"    WHERE s.setrole = %1\n "
					")\n "
					"SELECT datname, split_part(config, '=', 1) AS variable, \n"
					"       replace(config, split_part(config, '=', 1) || '=', '') AS value\n "
					"FROM configs";
	query = query.arg(_objectProperties.oid());
	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString datname = set->value("datname");
			QString variable = set->value("variable");
			QString value = set->value("value");
			PGKeyValueSetting setting;

			if (datname.isEmpty())
				setting.first = variable;
			else
				setting.first = QObject::tr("%1 (database %2)").arg(variable).arg(datname);
			setting.second = value;

			settingsArray.push_back(setting);

			set->moveNext();
		}
		delete set;
	}
	setObjectAttribute("variables", QVariant::fromValue(settingsArray));
}
