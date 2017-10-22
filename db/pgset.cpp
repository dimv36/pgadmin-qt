#include "db/pgset.h"


PGSet::PGSet()
: _connection(nullptr),
  _result(nullptr),
  _nColumns(0),
  _nRows(0),
  _position(0) {}

PGSet::PGSet(PGresult *result, PGConnection *connection)
: _connection(connection),
  _result(result),
  _nColumns(0),
  _nRows(0),
  _position(0)
{
	if (PQresultStatus(_result) == PGRES_TUPLES_OK)
	{
		_nColumns = PQnfields(_result);
		_nRows = PQntuples(_result);
	}
	moveFirst();
}

PGSet::~PGSet()
{
	if (_result)
		PQclear(_result);
	_result = nullptr;
}

void PGSet::moveFirst()
{
	if (_nRows)
		_position = 1;
	else
		_position = 0;
}

void PGSet::moveNext()
{
	if (_position <= _nRows)
		_position++;
}

void PGSet::movePrev()
{
	if (_position)
		_position--;
}

void PGSet::moveLast()
{
	_position = _nRows;
}

bool PGSet::bef() const
{
	return (!_nRows || _position < 1);
}

bool PGSet::eof() const
{
	return (!_nRows || _position > _nRows);
}

QString PGSet::columnName(const int column) const
{
	if (column > _nColumns)
		return QString();
	return QString(PQfname(_result, column));
}

int PGSet::columnNumber(const QString &columnName) const
{
	return PQfnumber(_result, qPrintable(columnName));
}

bool PGSet::columnIsNull(const int column) const
{
	return PQgetisnull(_result, _position - 1, column);
}

QString PGSet::value(const int column) const
{
	return QString(charPtr(column));
}

QString PGSet::value(const QString &column) const
{
	return QString(charPtr(column));
}

Oid PGSet::oidValue(const int column) const
{
	return QString(charPtr(column)).toInt();
}

Oid PGSet::oidValue(const QString &column) const
{
	return QString(charPtr(column)).toInt();
}

char *PGSet::charPtr(const int column) const
{
	return PQgetvalue(_result, _position - 1, column);
}

char *PGSet::charPtr(const QString &column) const
{
	return PQgetvalue(_result, _position - 1, columnNumber(column));
}
