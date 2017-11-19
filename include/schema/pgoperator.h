#ifndef PGOPERATOR_H
#define PGOPERATOR_H

#include "schema/pgobject.h"

class PGOperator : public PGObject
{
	Q_OBJECT
public:
	PGOperator(const PGConnection *connection, const QString &name);
	PGOperator(const PGConnection *connection, const Oid schemaOid);

	virtual QString fullName() const;
	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
};

#endif // PGOPERATOR_H
