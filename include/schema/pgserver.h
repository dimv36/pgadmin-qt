#ifndef PGSERVER_H
#define PGSERVER_H

#include <QString>

#include "pgobject.h"
#include "db/pgconnection.h"

class PGServer : public PGObject
{
	Q_OBJECT
public:
	PGServer(const QString &connectionName, const QString &host, const int port,
			 const QString &dbname, const QString &username, const QString &password);
	PGServer();

	void connect();
	bool connected() const;
	void disconnect();

	virtual void showSingleObjectProperties(PropertyTable *table);
	virtual void appendCollectionItems();
	virtual bool exists() const { return true; }

	QString connectionName() const;
	QString host() const;
	int port() const;
	QString dbname() const;
	QString username() const;
	QString password() const;

protected:
	virtual void formContextMenu(QMenu *menu);

private slots:
	void slotServerConnect();
	void slotServerReconnect();
	void slotServerDisconnect();
	void slotServerDelete();

private:
	QString _connectionName;
	QString _host;
	int _port;
	QString _dbname;
	QString _username;
	QString _password;
};

#endif // PGSERVER_H
