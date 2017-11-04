#ifndef PGUSER_H
#define PGUSER_H

#include "pgobject.h"

class PGUser : public PGObject
{
	Q_OBJECT
public:
	PGUser(const PGConnection *connection, const QString &name);
	PGUser(PGConnection *connection);

	void refreshObjectProperties(PropertyTable *);
};

#endif // PGUSER_H
