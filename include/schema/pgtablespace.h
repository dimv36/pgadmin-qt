#ifndef PGTABLESPACE_H
#define PGTABLESPACE_H

#include "pgobject.h"

class PGTablespace : public PGObject
{
	Q_OBJECT
public:
	PGTablespace(const QString &name);
	PGTablespace(PGConnection *connection);

	virtual PGTablespace* appendObject(const QString &);
	void refreshObjectProperties(PropertyTable *);
};

#endif // PGTABLESPACE_H
