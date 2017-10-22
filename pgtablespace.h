#ifndef PGTABLESPACE_H
#define PGTABLESPACE_H

#include "pgobject.h"

class PGTablespace : public PGObject
{
	Q_OBJECT
public:
	PGTablespace();
	~PGTablespace();

	void refreshProperties(PropertyTable *);
};

#endif // PGTABLESPACE_H
