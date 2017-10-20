#include <QMessageBox>
#include "pgserver.h"
#include "pgobject.h"
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
	setIcon(ColumnText, QIcon(":/server-connected.png"));
}

bool PGServer::connected() const
{
	if (_objtype == OBJECT_SERVER)
		return _connection->connected();
	else
		return false;
}

void PGServer::setMainObjectProperties(PropertyTable *tab)
{
	bool isConnected = connected();

	tab->removeRows();
	tab->addRow(QObject::tr("Name"), _connectionName);
	tab->addRow(QObject::tr("Host"), _host);
	tab->addRow(QObject::tr("Port"), QString::number(_port));
	tab->addRow(QObject::tr("Database"), _dbname);
	tab->addRow(QObject::tr("Username"), _username);
	tab->addRow(QObject::tr("Connected?"), isConnected);
	if (isConnected)
	{

	}
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
	if (_connection->connected())
		_connection->disconnect();
	setIcon(ColumnText, QIcon(":/server-disconnected.png"));
}
