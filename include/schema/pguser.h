#ifndef PGUSER_H
#define PGUSER_H

#include "pgrole.h"

class PGUser : public PGRole
{
	Q_OBJECT
public:
	PGUser(const PGConnection *connection, const QString &name);
	PGUser(const PGConnection *connection);
};

#endif // PGUSER_H
