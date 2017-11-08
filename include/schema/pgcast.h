#ifndef PGCAST_H
#define PGCAST_H

#include "schema/pgobject.h"

class PGCast : public PGObject
{
	Q_OBJECT
public:
	PGCast(const PGConnection *connection, const QString &name);
	PGCast(const PGConnection *connection);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;
};

#endif // PGCAST_H
