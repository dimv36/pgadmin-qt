#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgobject.h"

class PGGroup : public PGObject
{
public:
	PGGroup();
	virtual void refreshProperties(PropertyTable *);
};

#endif // PGGROUP_H
