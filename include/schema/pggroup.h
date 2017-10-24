#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgobject.h"

class PGGroup : public PGObject
{
public:
	PGGroup(const PGConnection *connection, const QString &name);
	PGGroup(PGConnection *connection);
	virtual void refreshObjectProperties(PropertyTable *);

	virtual PGGroup* appendObject(const PGConnection *connection, const QString &);
};

#endif // PGGROUP_H
