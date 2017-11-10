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
		   QIcon(":/server-disconnected"))
{
	_connection = new PGConnection(host, port, dbname, username, password);

	setObjectAttribute("name", connectionName);
	setObjectAttribute("host", host);
	setObjectAttribute("port", port);
	setObjectAttribute("dbname", dbname);
	setObjectAttribute("username", username);
	setObjectAttribute("password", password);
}

PGServer::PGServer()
: PGObject(nullptr, COLLECTION_SERVERS, QObject::tr("Servers"), QIcon(":/servers"))
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
	setIcon(ColumnText, QIcon(":/server-connected"));
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

	tab->addRow(QObject::tr("Connection name"), _objectProperties.stringValue("name"));
	tab->addRow(QObject::tr("Hostname"), _objectProperties.stringValue("host"));
	tab->addRow(QObject::tr("Port"), _objectProperties.intValue("port"));
	tab->addRow(QObject::tr("Maintenance database"), _objectProperties.stringValue("database"));
	tab->addRow(QObject::tr("Username"), _objectProperties.stringValue("username"));
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
	if (isCollectionItem())
		return;
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

void PGServer::refresh()
{
	if (!isCollectionItem())
		appendOrRefreshObject(this);
	else
	{
		appendOrRefreshObject();
		refreshCollectionTitle();
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
	setIcon(ColumnText, QIcon(":/server-disconnected"));
	removeChildrens();
}

void PGServer::slotServerDelete()
{
	slotServerDisconnect();
	delete this;
}

QString PGServer::password() const
{
	return _objectProperties.stringValue("password");
}

QString PGServer::username() const
{
	return _objectProperties.stringValue("username");
}

QString PGServer::dbname() const
{
	return _objectProperties.stringValue("dbname");
}

int PGServer::port() const
{
	return _objectProperties.intValue("port");
}

QString PGServer::host() const
{
	return _objectProperties.stringValue("host");
}

QString PGServer::connectionName() const
{
	return _objectProperties.stringValue("name");
}
