#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <libpq-fe.h>
#include <QString>
#include <QMutex>

class PGConnection
{
public:
	PGConnection(const QString &host, const int port,
				 const QString &dbname, const QString &username, const QString &password);
	~PGConnection();

	QString connectionString() const;
	bool connected() const;

	bool connect();
	void disconnect();
	void reconnect();

	QString lastError() const;

	void setCancel();
	void cancelQuery();
	void resetCancel();

private:
	void close();
	bool initialize();

private:
	QString _host;
	int _port;
	QString _dbname;
	QString _username;
	QString _password;
	QMutex *_cancelMutex;
	QString _connstr;

	PGconn *_connection;
	PGcancel *_cancel;
};

#endif // PGCONNECTION_H
