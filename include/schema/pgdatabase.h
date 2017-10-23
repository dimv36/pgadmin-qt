#ifndef PGDATABASE_H
#define PGDATABASE_H

#include "pgobject.h"

class PGDatabase : public PGObject
{
	Q_OBJECT
public:
	PGDatabase(const QString &name);
	PGDatabase(PGConnection *connection);

	virtual PGDatabase* appendObject(const QString &);
	virtual void refreshObjectProperties(PropertyTable *);
};

#endif // PGDATABASE_H
