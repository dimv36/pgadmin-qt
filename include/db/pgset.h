#ifndef PGSET_H
#define PGSET_H

#include <libpq-fe.h>
#include <QString>

class PGConnection;

class PGSet
{

public:
	PGSet();
	PGSet(PGresult *result, PGConnection *connection);
	~PGSet();

	int rowsCount() const;
	int columnsCount() const;
	int currentPosition() const;

	void moveFirst();
	void moveNext();
	void movePrev();
	void moveLast();

	bool bef() const;
	bool eof() const;

	QString columnName(const int column) const;
	int columnNumber(const QString &columnName) const;
	bool columnIsNull(const int column) const;
	bool hasColumn(const QString name) const;

	QString value(const int column) const;
	QString value(const QString &column) const;

	Oid oidValue(const int column) const;
	Oid oidValue(const QString &column) const;

	char *charPtr(const int column) const;
	char *charPtr(const QString &column) const;

private:
	PGConnection *_connection;
	PGresult *_result;
	int _nColumns;
	int _nRows;
	int _position;
};

#endif // PGSET_H
