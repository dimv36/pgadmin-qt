#ifndef PGFOREIGNDATAWRAPPER_H
#define PGFOREIGNDATAWRAPPER_H

#include "schema/pgobject.h"

class PGForeignDataWrapper : public PGObject
{
	Q_OBJECT
public:
	PGForeignDataWrapper(const PGConnection *connection, const QString &name);
	PGForeignDataWrapper(const PGConnection *connection);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;
};

#endif // PGFOREIGNDATAWRAPPER_H
