#ifndef PGDATABASE_H
#define PGDATABASE_H

#include "pgobject.h"

class PGDatabase : public PGObject
{
	Q_OBJECT
public:
	PGDatabase(const QString &name);
	PGDatabase();

	void refreshProperties(PropertyTable *);
};

#endif // PGDATABASE_H
