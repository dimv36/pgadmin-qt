#include <QMessageBox>
#include "pgserver.h"
#include "pgdatabase.h"
#include "pgtablespace.h"
#include "pggroup.h"
#include "pguser.h"
#include "propertytable.h"


PGServer::PGServer(const QString &connectionName,
				   const QString &host,
				   const int port,
				   const QString &dbname,
				   const QString &username,
				   const QString &password)
: PGObject(OBJECT_SERVER,
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
}

PGServer::PGServer()
: PGObject(COLLECTION_SERVERS,
		   QObject::tr("Servers"),
		   QIcon(":/servers.png"))
{
	_connection = nullptr;
}

PGServer::~PGServer()
{

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
	emit signalDataChanged(this);
}

bool PGServer::connected() const
{
	if (_objtype == OBJECT_SERVER)
		return _connection->connected();
	else
		return false;
}

void PGServer::refreshProperties(PropertyTable *tab)
{
	if (_objtype == COLLECTION_SERVERS)
		return;

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
	addChild(new PGDatabase());
	addChild(new PGTablespace());
	addChild(new PGUser());
	addChild(new PGGroup());
}

void PGServer::formContextMenu(QMenu *menu)
{
	if (_connection->connected())
	{
		PGObject::formContextMenu(menu);
		menu->addAction(QObject::tr("Disconnect"), this, SLOT(slotServerDisconnect()));
		menu->addAction(QObject::tr("Reconnect"), this, SLOT(slotServerReconnect()));
	}
	else
	{
		menu->addAction(QObject::tr("Connect"), this, SLOT(slotServerConnect()));
	}
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
	emit signalDataChanged(this);
}
