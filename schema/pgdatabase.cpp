#include <QMessageBox>
#include "schema/pgdatabase.h"
#include "schema/pgcast.h"
#include "schema/pgschema.h"

PGDatabase::PGDatabase(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_DATABASE, name, QIcon(":/database.png"))
{
	if (!(connection->databaseName() == name))
		setIcon(ColumnText, QIcon(":/database-disconnected.png"));
	else
		connect();
}

PGDatabase::PGDatabase(const PGConnection *connection)
: PGObject(connection, COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"), QIcon(":/database.png"))
{}

void PGDatabase::connect()
{
	if (_connection->databaseName() != _objectProperties.name())
	{
		_connection->disconnect();
		_connection->setDatabaseName(_objectProperties.name());
	}
	if (!_connection->connected())
	{
		bool success = _connection->connect();
		if (success)
		{
			setIcon(ColumnText, QIcon(":/database.png"));
			setDefaultParams();
		}
		else
		{
			setIcon(ColumnText, QIcon(":/database-disconnected.png"));
			QMessageBox::critical(nullptr,
								  QObject::tr("Connection failed"),
								  _connection->lastError());
			return;
		}
	}
	appendCollectionItems();
}

bool PGDatabase::connected() const
{
	return ((_connection->databaseName() == _objectProperties.name()) &&
			_connection->connected());
}

void PGDatabase::disconnect()
{
	_connection->disconnect();
	setIcon(ColumnText, QIcon(":/database-disconnected.png"));
}

void PGDatabase::appendCollectionItems()
{
	addChild(newPGObject<PGCast>(_connection));
	addChild(newPGObject<PGSchema>(_connection));
}

void PGDatabase::appendOrRefreshObject(PGObject *object)
{
	PGDatabase *database = nullptr;

	QString query = "SELECT db.oid AS oid, datname, db.dattablespace AS spcoid, datallowconn, spcname, datacl, \n"
					"pg_encoding_to_char(encoding) AS server_encoding, pg_get_userbyid(datdba) AS datowner, \n"
					"has_database_privilege(db.oid, 'CREATE') AS cancreate, \n"
					"current_setting('default_tablespace') AS default_tablespace, \n"
					"descr.description AS comment, db.datconnlimit AS connection_limit, \n"
					"db.datctype AS ctype, db.datcollate AS collate, \n"
					"(SELECT array_agg(label) FROM pg_shseclabel sl1 WHERE sl1.objoid = db.oid) AS labels, \n"
					"(SELECT array_agg(provider) FROM pg_shseclabel sl2 WHERE sl2.objoid=db.oid) AS providers \n"
					"  FROM pg_database db\n"
					"  LEFT OUTER JOIN pg_tablespace ta ON db.dattablespace=ta.OID\n"
					"  LEFT OUTER JOIN pg_shdescription descr ON (db.oid=descr.objoid AND descr.classoid='pg_database'::regclass) \n";
	if (object)
		query += QString("WHERE db.oid = %1").arg(object->oidObjectAttribute("oid"));
	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString datname = set->value("datname");

			if (!object)
				database = new PGDatabase(_connection, datname);
			else
				database = dynamic_cast<PGDatabase *>(object);
			database->setObjectAttribute("oid", set->oidValue("oid"));
			database->setObjectAttribute("owner", set->value("datowner"));
			database->setObjectAttribute("comment", set->value("comment"));
			database->setObjectAttribute("acl", set->value("datacl"));
			database->setObjectAttribute("spcname", set->value("spcname"));
			database->setObjectAttribute("default_tablespace", set->value("default_tablespace"));
			database->setObjectAttribute("encoding", set->value("server_encoding"));
			database->setObjectAttribute("collate", set->value("collate"));
			database->setObjectAttribute("ctype", set->value("ctype"));
			database->setObjectAttribute("allow_connections", set->boolValue("datallowconn"));
			database->setObjectAttribute("connection_limit", set->intValue("connection_limit"));

			database->parseSecurityLabels(set->value("providers"), set->value("labels"));

			if (!object)
				addChild(database);

			set->moveNext();
		}
		delete set;
	}
}

void PGDatabase::showSingleObjectProperties(PropertyTable *tab)
{
	getDatabaseSettings();

	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Tablespace"), _objectProperties.stringValue("spcname"));
	tab->addRow(QObject::tr("Default tablespace"), _objectProperties.stringValue("default_tablespace"));
	tab->addRow(QObject::tr("Encoding"), _objectProperties.stringValue("serverencoding"));
	tab->addRow(QObject::tr("Collation"), _objectProperties.stringValue("collate"));
	tab->addRow(QObject::tr("Character type"), _objectProperties.stringValue("ctype"));
	tab->addRow(QObject::tr("Search path"), _searchPath);
	tab->addRow(QObject::tr("Default table ACL"), _defPrivsOnTables);
	tab->addRow(QObject::tr("Default sequence ACL"), _defPrivsOnSeqs);
	tab->addRow(QObject::tr("Default function ACL"), _defPrivsOnFuncs);
	tab->addRow(QObject::tr("Default type ACL"), _defPrivsOnFuncs);
	tab->addRow(QObject::tr("Allow connections?"), _objectProperties.boolValue("allow_connections"));
	tab->addRow(QObject::tr("Connected?"), connected());
	tab->addRow(QObject::tr("Connection limit"), _objectProperties.intValue("connection_limit"));
	tab->addRow(QObject::tr("System database?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	appendSecurityLabels(tab);
	// Append settings
	PGKeyValueSettings settings = _objectProperties.variables();
	for (auto it = settings.begin(); it != settings.end(); ++it)
	{
		PGKeyValueSetting setting = *it;
		tab->addRow(setting.first, setting.second, QIcon(":/variable.png"));
	}
}

bool PGDatabase::exists() const
{
	if (IsCollectionItem(_objtype))
		return true;
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_database WHERE oid = %1)")
										  .arg(_objectProperties.oidString()));
}

void PGDatabase::slotReconnect()
{
	slotDisconnect();
	slotConnect();
}

void PGDatabase::slotConnect()
{
	connect();
}

void PGDatabase::slotDisconnect()
{
	disconnect();
}

void PGDatabase::formContextMenu(QMenu *menu)
{
	PGObject::formContextMenu(menu);
	if (!IsCollectionItem(_objtype))
	{
		if (connected())
		{
			menu->addAction(QObject::tr("Disconnect"), this, SLOT(slotDisconnect()));
			menu->addAction(QObject::tr("Reconnect"), this, SLOT(slotReconnect()));
		}
		else
		{
			menu->addAction(QObject::tr("Connect"), this, SLOT(slotConnect()));
		}
	}
}

void PGDatabase::getDatabaseSettings()
{
	PGKeyValueSettings settingsArray;

	QString query = "WITH configs\n"
					"AS\n "
					"(\n"
					"    SELECT rolname, unnest(setconfig) AS config\n "
					"    FROM pg_db_role_setting s\n "
					"    LEFT JOIN pg_roles r ON r.oid = s.setrole\n "
					"    WHERE s.setdatabase = %1\n "
					")\n "
					"SELECT rolname, split_part(config, '=', 1) AS variable, \n"
					"       replace(config, split_part(config, '=', 1) || '=', '') AS value\n "
					"FROM configs";
	query = query.arg(_objectProperties.oid());
	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString username = set->value("rolname");
			QString variable = set->value("variable");
			QString value = set->value("value");
			PGKeyValueSetting setting;

			if (username.isEmpty())
				setting.first = variable;
			else
				setting.first = QObject::tr("%1 (role %2)").arg(variable).arg(username);
			setting.second = value;

			settingsArray.push_back(setting);

			set->moveNext();
		}
		delete set;
	}
	setObjectAttribute("variables", QVariant::fromValue(settingsArray));
}

void PGDatabase::setDefaultParams()
{
	_searchPath = _connection->executeScalar("SHOW search_path");
	_defPrivsOnTables = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='r'");
	_defPrivsOnSeqs = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='S'");
	_defPrivsOnFuncs = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='f'");
	_defPrivsOnTypes = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='T'");
}
