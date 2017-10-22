#ifndef PGSERVER_H
#define PGSERVER_H

#include <QString>

#include "pgobject.h"
#include "pgconnection.h"

class PGServer : public PGObject
{
	Q_OBJECT
public:
	PGServer(const QString &connectionName, const QString &host, const int port,
			 const QString &dbname, const QString &username, const QString &password);
	PGServer();
	~PGServer();

	void connect();
	bool connected() const;
	void disconnect();

	virtual void refreshObjectProperties(PropertyTable *table);
	virtual void appendCollectionItems();

protected:
	virtual void formContextMenu(QMenu *menu);

private slots:
	void slotServerConnect();
	void slotServerReconnect();
	void slotServerDisconnect();

private:
	QString _connectionName;
	QString _host;
	int _port;
	QString _dbname;
	QString _username;
};

#endif // PGSERVER_H
