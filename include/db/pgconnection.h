#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <libpq-fe.h>
#include <QString>
#include <QMutex>
#include "pgset.h"
#include "includes.h"

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

	void setError(PGresult *result = nullptr);
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

	void notice(const char *message);

	QString versionString() const;
	QString version() const;

	QString databaseName() const;
	void setDatabaseName(const QString &name);

	Oid databaseOid() const;
	QString encoding() const;
	Oid lastSystemOid() const;

protected:
	void setLastResultError(PGresult *result = nullptr, const QString &error = QString());

private:
	void close();
	bool initialize();
	QString dbString(const QString &str);
	void formConnectionString();

protected:
	void *_noticeArg;
	PQnoticeProcessor _noticeProc;

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
	PGError _lastResultError;

	QString _versionNum;
	QString _versionStr;
	int _major;
	int _minor;

	Oid _dbOid;
	QString _encoding;
	Oid _lastSystemOid;
};

#endif // PGCONNECTION_H
