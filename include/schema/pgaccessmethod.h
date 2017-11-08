#ifndef PGACCESSMETHOD_H
#define PGACCESSMETHOD_H

#include "schema/pgobject.h"

class PGAccessMethod : public PGObject
{
	Q_OBJECT
public:
	PGAccessMethod(const PGConnection *connection, const QString &name);
	PGAccessMethod(const PGConnection *connection);

	virtual bool exists() const;
	virtual void appendOrRefreshObject(PGObject *object = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
};

#endif // PGACCESSMETHOD_H
