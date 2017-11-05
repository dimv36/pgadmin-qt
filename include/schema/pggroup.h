#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgrole.h"

class PGGroup : public PGRole
{
public:
	PGGroup(const PGConnection *connection, const QString &name);
	PGGroup(PGConnection *connection);
	virtual void refreshObjectProperties(PropertyTable *);
};

#endif // PGGROUP_H
