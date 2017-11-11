#ifndef PGFOREIGNSERVER_H
#define PGFOREIGNSERVER_H

#include "schema/pgobject.h"

class PGForeignServer : public PGObject
{
	Q_OBJECT
public:
	PGForeignServer(const PGConnection *connection, const QString &name);
	PGForeignServer(const PGConnection *connection, const Oid fdwOid);

	virtual void appendCollectionItems();
	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _fdwOid;
};

#endif // PGFOREIGNSERVER_H
