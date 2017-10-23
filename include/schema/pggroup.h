#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgobject.h"

class PGGroup : public PGObject
{
public:
	PGGroup(const QString &name);
	PGGroup(PGConnection *connection);
	virtual void refreshObjectProperties(PropertyTable *);

	virtual PGGroup* appendObject(const QString &);
};

#endif // PGGROUP_H
