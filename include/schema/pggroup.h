#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgobject.h"

class PGGroup : public PGObject
{
public:
	PGGroup(PGConnection *connection);
	virtual void refreshObjectProperties(PropertyTable *);
};

#endif // PGGROUP_H
