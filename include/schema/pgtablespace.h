#ifndef PGTABLESPACE_H
#define PGTABLESPACE_H

#include "pgobject.h"

class PGTablespace : public PGObject
{
	Q_OBJECT
public:
	PGTablespace(const PGConnection *connection, const QString &name);
	PGTablespace(const PGConnection *connection);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
//	virtual void refresh();
	virtual void showSingleObjectProperties(PropertyTable *);
};

#endif // PGTABLESPACE_H
