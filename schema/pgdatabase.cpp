#include "schema/pgdatabase.h"

PGDatabase::PGDatabase(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_DATABASE, name, QIcon(":/database.png"))
{
	if (!(connection->databaseName() == name))
		setIcon(ColumnText, QIcon(":/database-disconnected.png"));
}

PGDatabase::PGDatabase(PGConnection *connection)
: PGObject(connection, COLLECTION_DATABASES, QObject::tr("Databases"), QIcon(":/databases.png"), QIcon(":/database.png"))
{
	_propertiesSQL = "SELECT dat.datname AS objname,\n"
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
		// TODO: raise error dialog iff not connected
		if (success)
			setIcon(ColumnText, QIcon(":/database.png"));
		else
			setIcon(ColumnText, QIcon(":/database-disconnected.png"));
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
