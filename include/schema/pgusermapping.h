#ifndef PGUSERMAPPING_H
#define PGUSERMAPPING_H

#include "schema/pgobject.h"

class PGUserMapping : public PGObject
{
	Q_OBJECT
public:
	PGUserMapping(const PGConnection *connection, const QString &name);
	PGUserMapping(const PGConnection *connection, const Oid foreignServerOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _foreignServerOid;
};

#endif // PGUSERMAPPING_H
