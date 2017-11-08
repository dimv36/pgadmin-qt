#ifndef PGEXTENSION_H
#define PGEXTENSION_H

#include "schema/pgobject.h"

class PGExtension : public PGObject
{
	Q_OBJECT
public:
	PGExtension(const PGConnection *connection, const QString &name);
	PGExtension(const PGConnection *connection);

	virtual bool exists() const;
	virtual void appendOrRefreshObject(PGObject *object = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
};

#endif // PGEXTENSION_H
