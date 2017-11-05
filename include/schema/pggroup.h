#ifndef PGGROUP_H
#define PGGROUP_H

#include "pgrole.h"

class PGGroup : public PGRole
{
public:
	PGGroup(const PGConnection *connection, const QString &name);
	PGGroup(const PGConnection *connection);
};

#endif // PGGROUP_H
