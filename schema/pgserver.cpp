#include <QMessageBox>
#include "schema/pgserver.h"
#include "schema/pgdatabase.h"
#include "schema/pgtablespace.h"
#include "schema/pggroup.h"
#include "schema/pguser.h"
#include "widget/propertytable.h"


PGServer::PGServer(const QString &connectionName,
				   const QString &host,
				   const int port,
				   const QString &dbname,
				   const QString &username,
				   const QString &password)
: PGObject(nullptr,
		   OBJECT_SERVER,
		   QString("%1 (%2:%3)").arg(connectionName)
								.arg(host.isEmpty() ? QObject::tr("local") : host)
								.arg(QString::number(port)),
		   QIcon(":/server-disconnected.png"))
{
	_connection = new PGConnection(host, port, dbname, username, password);

	// Set server properties
	_connectionName = connectionName;
	_host = host;
	_port = port;
	_dbname = dbname;
	_username = username;
	_password = password;
}

PGServer::PGServer()
: PGObject(nullptr,
		   COLLECTION_SERVERS,
		   QObject::tr("Servers"),
		   QIcon(":/servers.png"))
{
	_connection = nullptr;
}

void PGServer::connect()
{
	if (!_connection->connect())
	{
		QMessageBox::critical(0,
							  QObject::tr("Connection failed to %1").arg(_connection->connectionString()),
							  _connection->lastError());
		return;
	}
	appendCollectionItems();
	setIcon(ColumnText, QIcon(":/server-connected.png"));
	treeWidget()->expandItem(this);
}

bool PGServer::connected() const
{
	if (_objtype == OBJECT_SERVER)
		return _connection->connected();
	else
		return false;
}

void PGServer::showSingleObjectProperties(PropertyTable *tab)
{
	bool isConnected = connected();

	tab->addRow(QObject::tr("Description"), _connectionName);
	tab->addRow(QObject::tr("Hostname"), _host);
	tab->addRow(QObject::tr("Port"), QString::number(_port));
	tab->addRow(QObject::tr("Maintenance database"), _dbname);
	tab->addRow(QObject::tr("Username"), _username);
	tab->addRow(QObject::tr("Connected?"), isConnected);
	if (isConnected)
	{
		tab->addRow(QObject::tr("Server version"), _connection->version());
		tab->addRow(QObject::tr("Server version string"), _connection->versionString());
		tab->addRow(QObject::tr("Encoding"), _connection->encoding());
		tab->addRow(QObject::tr("Last system OID"), QString::number(_connection->lastSystemOid()));
	}
}

void PGServer::appendCollectionItems()
{
	addChild(newPGObject<PGDatabase>(_connection));
	addChild(newPGObject<PGTablespace>(_connection));
	addChild(newPGObject<PGGroup>(_connection));
	addChild(newPGObject<PGUser>(_connection));
}

void PGServer::formContextMenu(QMenu *menu)
{
	if (_connection && _connection->connected())
	{
		PGObject::formContextMenu(menu);
		menu->addAction(QObject::tr("Disconnect"), this, SLOT(slotServerDisconnect()));
		menu->addAction(QObject::tr("Reconnect"), this, SLOT(slotServerReconnect()));
	}
	else
	{
		menu->addAction(QObject::tr("Connect"), this, SLOT(slotServerConnect()));
	}
	menu->addAction(QObject::tr("Delete"), this, SLOT(slotServerDelete()));
}

void PGServer::slotServerConnect()
{
	connect();
}

void PGServer::slotServerReconnect()
{
	slotServerDisconnect();
	slotServerConnect();
}

void PGServer::slotServerDisconnect()
{
	_connection->disconnect();
	setIcon(ColumnText, QIcon(":/server-disconnected.png"));
	removeChildrens();
}

void PGServer::slotServerDelete()
{
	delete this;
}

QString PGServer::password() const
{
	return _password;
}

QString PGServer::username() const
{
	return _username;
}

QString PGServer::dbname() const
{
	return _dbname;
}

int PGServer::port() const
{
	return _port;
}

QString PGServer::host() const
{
	return _host;
}

QString PGServer::connectionName() const
{
	return _connectionName;
}
