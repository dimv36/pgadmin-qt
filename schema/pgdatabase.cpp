#include <QMessageBox>
#include "schema/pgdatabase.h"

PGDatabase::PGDatabase(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_DATABASE, name, QIcon(":/database.png")),
  _missingFk(0)
{
	if (!(connection->databaseName() == name))
		setIcon(ColumnText, QIcon(":/database-disconnected.png"));
}

PGDatabase::PGDatabase(PGConnection *connection)
: PGObject(connection, COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"), QIcon(":/database.png"))
{
	_propertiesSQL = "SELECT dat.oid AS oid, dat.datname AS objname,\n"
					"        pg_get_userbyid(datdba) AS owner,\n"
					 "       des.description AS comment\n"
					 "FROM pg_database dat\n"
					 "LEFT JOIN pg_shdescription des\n"
					 "ON dat.oid = des.objoid\n"
					 "ORDER by dat.oid;";
}

void PGDatabase::connect()
{
	if (_connection->databaseName() != _name)
	{
		_connection->disconnect();
		_connection->setDatabaseName(_name);
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
		}
	}
}

bool PGDatabase::connected() const
{
	return ((_connection->databaseName() == _name) &&
			_connection->connected());
}

void PGDatabase::disconnect()
{
	_connection->disconnect();
	setIcon(ColumnText, QIcon(":/database-disconnected.png"));
}

PGDatabase *PGDatabase::appendObject(const PGConnection *connection, const QString &name)
{
	return new PGDatabase(connection, name);
}

void PGDatabase::refreshObjectProperties(PropertyTable *tab)
{
	tab->setHeaders();

	QString query = "SELECT db.oid, datname, db.dattablespace AS spcoid, spcname, datacl, "
					"pg_encoding_to_char(encoding) AS serverencoding, pg_get_userbyid(datdba) AS datowner,"
					"has_database_privilege(db.oid, 'CREATE') AS cancreate, \n"
					"current_setting('default_tablespace') AS default_tablespace, \n"
					"descr.description AS comment, db.datconnlimit AS connectionlimit, \n"
					"db.datctype AS ctype, db.datcollate AS collate, \n"
					"(SELECT array_agg(label) FROM pg_shseclabel sl1 WHERE sl1.objoid = db.oid) AS labels, \n"
					"(SELECT array_agg(provider) FROM pg_shseclabel sl2 WHERE sl2.objoid=db.oid) AS providers \n"
					"  FROM pg_database db\n"
					"  LEFT OUTER JOIN pg_tablespace ta ON db.dattablespace=ta.OID\n"
					"  LEFT OUTER JOIN pg_shdescription descr ON (db.oid=descr.objoid AND descr.classoid='pg_database'::regclass)\n"
					"  WHERE datname=" + QString("'%1'").arg(_name);

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		tab->addRow(QObject::tr("Name"), _name);
		tab->addRow(QObject::tr("OID"), set->value("oid"));
		tab->addRow(QObject::tr("Owner"), set->value("datowner"));
		tab->addRow(QObject::tr("ACL"), set->value("datacl"));
		tab->addRow(QObject::tr("Tablespace"), set->value("spcname"));
		tab->addRow(QObject::tr("Default tablespace"), set->value("default_tablespace"));
		tab->addRow(QObject::tr("Encoding"), set->value("serverencoding"));
		tab->addRow(QObject::tr("Collation"), set->value("collate"));
		tab->addRow(QObject::tr("Character type"), set->value("ctype"));
		tab->addRow(QObject::tr("Search path"), _searchPath);
		tab->addRow(QObject::tr("Default table ACL"), _defPrivsOnTables);
		tab->addRow(QObject::tr("Default sequence ACL"), _defPrivsOnSeqs);
		tab->addRow(QObject::tr("Default function ACL"), _defPrivsOnFuncs);
		tab->addRow(QObject::tr("Default type ACL"), _defPrivsOnFuncs);
//		tab->addRow(QObject::tr("Allow connections?"), _allowConnections);
		tab->addRow(QObject::tr("Connected?"), connected());
		tab->addRow(QObject::tr("Connection limit"), set->value("connectionlimit"));
		tab->addRow(QObject::tr("System database?"), isSystemObject());
		tab->addRow(QObject::tr("Comment"), set->value("comment"));
	}
	delete set;
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
	if (connected())
	{
		PGObject::formContextMenu(menu);
		menu->addAction(QObject::tr("Disconnect"), this, SLOT(slotDisconnect()));
		menu->addAction(QObject::tr("Reconnect"), this, SLOT(slotReconnect()));
	}
	else
	{
		menu->addAction(QObject::tr("Connect"), this, SLOT(slotConnect()));
	}
}

void PGDatabase::setDefaultParams()
{
	_searchPath = _connection->executeScalar("SHOW search_path");
	_defPrivsOnTables = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='r'");
	_defPrivsOnSeqs = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='S'");
	_defPrivsOnFuncs = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='f'");
	_defPrivsOnTypes = _connection->executeScalar("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = 0::OID AND defaclobjtype='T'");
}
