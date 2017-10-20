#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <libpq-fe.h>
#include <QString>
#include <QMutex>

class PGSet;

typedef struct PGError
{
	QString severity;
	QString sqlState;
	QString messagePrimary;
	QString messageDetail;
	QString messageHint;
	QString statementPosition;
	QString internalPosition;
	QString internalQuery;
	QString context;
	QString sourceFile;
	QString sourceLine;
	QString sourceFunction;
	QString formattedMessage;

	void setError(PGresult *result = NULL);
} PGError;

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

	QString executeScalar(const QString &query);
	bool executeVoid(const QString &query);
	PGSet *executeSet(const QString &query);

	QString lastError() const;

	void setCancel();
	void cancelQuery();
	void resetCancel();

protected:
	void setLastResultError(PGresult *result = NULL, const QString &error = QString());

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

	bool _needColumnQuoting;

	PGconn *_connection;
	PGcancel *_cancel;
	PGError _lastResultError;
};

#endif // PGCONNECTION_H
