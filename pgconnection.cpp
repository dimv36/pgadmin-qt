#include <QMutexLocker>
#include "pgconnection.h"
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
		{
//			SetLastResultError(NULL, wxT("Cancel request sent"));
		}
		else
		{
//			SetLastResultError(NULL, wxString::Format(wxT("Could not send cancel request:\n%s"), errbuf));
		}
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

