#include "pgconnection.h"
#include "pgset.h"
#include <QMutexLocker>
#include <QMessageBox>
#include <QDebug>

static void pgNoticeProcessor(void *arg, const char *message)
{
	((PGConnection *) arg)->notice(message);
}


PGConnection::PGConnection(const QString &host, const int port,
						   const QString &dbname, const QString &username, const QString &password)
: _host(host),
  _port(port),
  _dbname(dbname),
  _username(username),
  _password(password),
  _cancelMutex(new QMutex())
{
	if (!_host.isEmpty())
		_connstr += QString(" host=%1").arg(_host);
	if (_port)
		_connstr += QString(" port=%1").arg(QString::number(_port));
	if (!_dbname.isEmpty())
		_connstr += QString(" dbname=%1").arg(_dbname);
	if (!_username.isEmpty())
		_connstr += QString(" user=%1").arg(_username);
	if (!_password.isEmpty())
		_connstr += QString(" password=%1").arg(_password);
	_connection = nullptr;
	_cancel = nullptr;
	_noticeArg = nullptr;
	_noticeProc = nullptr;
}

PGConnection::~PGConnection()
{
	close();
}

QString PGConnection::connectionString() const
{
	return _connstr;
}

bool PGConnection::connected() const
{
	return (_connection && (PQstatus(_connection) == CONNECTION_OK));
}

bool PGConnection::connect()
{
	_connection = PQconnectdb(qPrintable(_connstr));
	if (PQstatus(_connection) != CONNECTION_OK)
		return false;

	if (!initialize())
		return false;
	return true;
}

void PGConnection::disconnect()
{
	if (connected())
		close();
}

void PGConnection::reconnect()
{
	disconnect();
	connect();
}

QString PGConnection::executeScalar(const QString &query)
{
	QString result;

	if (connected())
	{
		PGresult *res = nullptr;

		setCancel();
		res = PQexec(_connection, qPrintable(query));
		resetCancel();

		ExecStatusType execResult = PQresultStatus(res);
		setLastResultError(res);

		if (execResult != PGRES_TUPLES_OK && execResult != PGRES_COMMAND_OK)
		{
			PQclear(res);
			return QString();
		}

		if (PQntuples(res) < 1)
		{
			PQclear(res);
			return QString();
		}

		result = QString(PQgetvalue(res, 0, 0));
		PQclear(res);
	}

	return result;
}

bool PGConnection::executeVoid(const QString &query)
{
	if (!connected())
		return false;

	PGresult *result = nullptr;

	setCancel();
	result = PQexec(_connection, qPrintable(query));
	resetCancel();

	ExecStatusType execResult = PQresultStatus(result);
	setLastResultError(result);

	if (execResult != PGRES_TUPLES_OK &&
		execResult != PGRES_COMMAND_OK)
	{
		PQclear(result);
		return false;
	}

	PQclear(result);

	return true;
}

PGSet *PGConnection::executeSet(const QString &query)
{
	if (connected())
	{
		PGresult *result;

		setCancel();
		result = PQexec(_connection, qPrintable(query));
		resetCancel();

		ExecStatusType execStatus = PQresultStatus(result);
		setLastResultError(result);

		if (execStatus == PGRES_TUPLES_OK || execStatus == PGRES_COMMAND_OK)
		{
			PGSet *set = new PGSet(result, this);
			if (!set)
				PQclear(result);
			return set;
		}
		else
			PQclear(result);
	}
	return new PGSet();
}

QString PGConnection::lastError() const
{
	char *error = nullptr;

	if (_connection && (error = PQerrorMessage(_connection)))
		return error;
	return QString();
}

void PGConnection::setCancel()
{
	QMutexLocker lock(_cancelMutex);
	PGcancel *oldCancelConn = _cancel;

	_cancel = nullptr;

	if (oldCancelConn)
		PQfreeCancel(oldCancelConn);

	if (!_connection)
		return;

	_cancel = PQgetCancel(_connection);
}

void PGConnection::cancelQuery()
{
	char		errbuf[256];
	QMutexLocker lock(_cancelMutex);
	Q_UNUSED(lock);

	if (_cancel)
	{
		PGcancel *cancelConn = _cancel;
		_cancel = nullptr;

		if (PQcancel(cancelConn, errbuf, sizeof(errbuf)))
			setLastResultError(nullptr, QObject::tr("Cancel request sent"));
		else
			setLastResultError(nullptr, QString("Could not send cancel request:\n%1").arg(errbuf));
		PQfreeCancel(cancelConn);
	}
}

void PGConnection::resetCancel()
{
	QMutexLocker lock(_cancelMutex);
	PGcancel *oldCancelConn = _cancel;

	_cancel = nullptr;

	if (oldCancelConn)
		PQfreeCancel(oldCancelConn);
}

void PGConnection::notice(const char *message)
{
	if (_noticeArg && _noticeProc)
		(*_noticeProc) (_noticeArg, message);
	else
	{
		QString str(message);

		// TODO: Опциональный показ замечаний
		QMessageBox::information(nullptr,
								 QObject::tr("Notice"),
								 str,
								 QMessageBox::Ok);
	}
}

QString PGConnection::versionString() const
{
	return _versionStr;
}

QString PGConnection::version() const
{
	return _versionNum;
}

QString PGConnection::databaseName() const
{
	return _dbname;
}

Oid PGConnection::databaseOid() const
{
	return _dbOid;
}

QString PGConnection::encoding() const
{
	return _encoding;
}

Oid PGConnection::lastSystemOid() const
{
	return _lastSystemOid;
}

void PGConnection::setLastResultError(PGresult *result, const QString &message)
{
	if (result)
	{
		_lastResultError.severity = QString(PQresultErrorField(result, PG_DIAG_SEVERITY));
		_lastResultError.sqlState = QString(PQresultErrorField(result, PG_DIAG_SQLSTATE));
		_lastResultError.messagePrimary = QString(PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY));
		_lastResultError.messageDetail = QString(PQresultErrorField(result, PG_DIAG_MESSAGE_DETAIL));
		_lastResultError.messageHint = QString(PQresultErrorField(result, PG_DIAG_MESSAGE_HINT));
		_lastResultError.statementPosition = QString(PQresultErrorField(result, PG_DIAG_STATEMENT_POSITION));
		_lastResultError.internalPosition = QString(PQresultErrorField(result, PG_DIAG_INTERNAL_POSITION));
		_lastResultError.internalQuery = QString(PQresultErrorField(result, PG_DIAG_INTERNAL_QUERY));
		_lastResultError.context = QString(PQresultErrorField(result, PG_DIAG_CONTEXT));
		_lastResultError.sourceFile = QString(PQresultErrorField(result, PG_DIAG_SOURCE_FILE));
		_lastResultError.sourceLine = QString(PQresultErrorField(result, PG_DIAG_SOURCE_LINE));
		_lastResultError.sourceFunction = QString(PQresultErrorField(result, PG_DIAG_SOURCE_FUNCTION));
	}
	else
	{
		_lastResultError.severity = QString();
		_lastResultError.sqlState = QString();
		if (message.isEmpty())
			_lastResultError.messagePrimary = lastError();
		else
			_lastResultError.messagePrimary = message;
		_lastResultError.messageDetail = QString();
		_lastResultError.messageHint = QString();
		_lastResultError.statementPosition = QString();
		_lastResultError.internalPosition = QString();
		_lastResultError.internalQuery = QString();
		_lastResultError.context = QString();
		_lastResultError.sourceFile = QString();
		_lastResultError.sourceLine = QString();
		_lastResultError.sourceFunction = QString();
	}

	QString errorMessage;

	if (_lastResultError.severity != QString() && _lastResultError.messagePrimary != QString())
		errorMessage = QString("%1: %2").arg(_lastResultError.severity, _lastResultError.messagePrimary);
	else if (!_lastResultError.messagePrimary.isEmpty())
		errorMessage = _lastResultError.messagePrimary;

	if (!_lastResultError.sqlState.isEmpty())
	{
		if (!errorMessage.endsWith('\n'))
			errorMessage += '\n';
		errorMessage += QObject::tr("SQL state: ");
		errorMessage += _lastResultError.sqlState;
	}

	if (!_lastResultError.messageDetail.isEmpty())
	{
		if (!errorMessage.endsWith('\n'))
			errorMessage += '\n';
		errorMessage += QObject::tr("Detail: ");
		errorMessage += _lastResultError.messageDetail;
	}

	if (!_lastResultError.messageHint.isEmpty())
	{
		if (!errorMessage.endsWith('\n'))
			errorMessage += '\n';
		errorMessage += QObject::tr("Hint: ");
		errorMessage += _lastResultError.messageHint;
	}

	if (!_lastResultError.statementPosition.isEmpty())
	{
		if (!errorMessage.endsWith('\n'))
			errorMessage += '\n';
		errorMessage += QObject::tr("Character: ");
		errorMessage += _lastResultError.statementPosition;
	}

	if (!_lastResultError.context.isEmpty())
	{
		if (!errorMessage.endsWith('\n'))
			errorMessage += '\n';
		errorMessage += QObject::tr("Context: ");
		errorMessage += _lastResultError.context;
	}
	_lastResultError.formattedMessage = errorMessage;
}

bool PGConnection::initialize()
{
	if (PQstatus(_connection) == CONNECTION_OK)
	{
		PQsetNoticeProcessor(_connection, pgNoticeProcessor, this);

		// Initialize sesssion settings
		executeVoid("SET DateStyle TO ISO;");
		executeVoid("SET client_min_messages TO 'notice';");
		executeVoid("SET bytea_output TO escape;");

		// Determine versions
		_versionStr = executeScalar("SELECT version();");
		int version = executeScalar("SELECT setting FROM pg_settings WHERE name = 'server_version_num';").toInt();

		// Determine actual version of server
		if (version < 100000)
		{
			// This is x.x.x server version so
			_major = version / 10000;
			_minor = (version  - (_major * 10000)) / 100;
			int patch = (version - (_major * 10000) - (_minor * 100));
			_versionNum = QString("%1.%2.%3").arg(_major).arg(_minor).arg(patch);
		}
		else
		{
			// This is x.x server version, so
			_major = version / 100000;
			_minor = version - (_major * 100000);
			_versionNum = QString("%1.%2").arg(_major).arg(_minor);
		}

		// We does not support any PG backends, that older 9.2
		if (_major < 9 || ((_major == 9) && (_minor < 2)))
			QMessageBox::warning(nullptr,
								 QObject::tr("PgAdmin"),
								 QObject::tr("Those version of PostgreSQL (%1) does not supported by PgAdmin.\n"
											 "Supported versions are: > 9.2"),
								 QMessageBox::Ok);

		QString sql = QString("SELECT oid, "
							  "       pg_encoding_to_char(encoding) AS encoding, "
							  "       datlastsysoid "
							  " FROM pg_database WHERE datname = %1").arg(dbString(_dbname));

		PGSet *set = executeSet(sql);
		if (set)
		{
			_dbOid = set->oidValue("oid");
			_encoding = set->value("encoding"),
			_lastSystemOid = set->oidValue("datlastsysoid");

			delete set;
		}

		return true;
	}
	return false;
}

QString PGConnection::dbString(const QString &str)
{
	QString result = str;

	result.replace("\\", "\\\\");
	result.replace("'", "\\'");
	return QString("'%1'").arg(result);
}

void PGConnection::close()
{
	if (_connection)
	{
		cancelQuery();
		PQfinish(_connection);
	}
	_connection = nullptr;
	_versionNum.clear();
	_versionStr.clear();
	_major = 0;
	_minor = 0;
}

