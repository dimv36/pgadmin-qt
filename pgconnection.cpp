#include "pgconnection.h"
#include "pgset.h"
#include <QMutexLocker>
#include <QDebug>


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
	_needColumnQuoting = false;
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
		PGresult *res = NULL;

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

	PGresult *result = NULL;

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
			PGSet *set = new PGSet(result, this, _needColumnQuoting);
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
	char *error = NULL;

	if (_connection && (error = PQerrorMessage(_connection)) != NULL)
	{
		return error;
	}
	return QString();
}

void PGConnection::setCancel()
{
	QMutexLocker lock(_cancelMutex);
	PGcancel *oldCancelConn = _cancel;

	_cancel = NULL;

	if (oldCancelConn != NULL)
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
		_cancel = NULL;

		if (PQcancel(cancelConn, errbuf, sizeof(errbuf)))
			setLastResultError(NULL, QObject::tr("Cancel request sent"));
		else
			setLastResultError(NULL, QString("Could not send cancel request:\n%1").arg(errbuf));
		PQfreeCancel(cancelConn);
	}
}

void PGConnection::resetCancel()
{
	QMutexLocker lock(_cancelMutex);
	PGcancel *oldCancelConn = _cancel;

	_cancel = NULL;

	if (oldCancelConn != NULL)
		PQfreeCancel(oldCancelConn);
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
	return true;
}

void PGConnection::close()
{
	if (_connection)
	{
		cancelQuery();
		PQfinish(_connection);
	}
	_connection = nullptr;
}

