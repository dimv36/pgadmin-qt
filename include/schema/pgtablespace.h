#ifndef PGTABLESPACE_H
#define PGTABLESPACE_H

#include "pgobject.h"

class PGTablespace : public PGObject
{
	Q_OBJECT
public:
	PGTablespace(PGConnection *connection);

	void refreshObjectProperties(PropertyTable *);
};

#endif // PGTABLESPACE_H
