#ifndef PGTABLESPACE_H
#define PGTABLESPACE_H

#include "pgobject.h"

class PGTablespace : public PGObject
{
	Q_OBJECT
public:
	PGTablespace(const PGConnection *connection, const QString &name);
	PGTablespace(PGConnection *connection);

	void refreshObjectProperties(PropertyTable *);
};

#endif // PGTABLESPACE_H
