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
	PGServer(bool isCollection = true);
	~PGServer();

	void connect();
	void disconnect();

protected:
	virtual void formContextMenu(QMenu *menu);

private slots:
	void slotServerConnect();
	void slotServerReconnect();
	void slotServerDisconnect();

private:
	PGConnection *_connection;
};

#endif // PGSERVER_H
