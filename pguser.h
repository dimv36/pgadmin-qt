#ifndef PGUSER_H
#define PGUSER_H

#include "pgobject.h"

class PGUser : public PGObject
{
	Q_OBJECT
public:
	PGUser();

	void refreshProperties(PropertyTable *);
};

#endif // PGUSER_H
