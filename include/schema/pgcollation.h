#ifndef PGCOLLATION_H
#define PGCOLLATION_H

#include "schema/pgobject.h"

class PGCollation : public PGObject
{
	Q_OBJECT
public:
	PGCollation(const PGConnection *connection, const QString &name);
	PGCollation(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
};

#endif // PGCOLLATION_H
