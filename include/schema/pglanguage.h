#ifndef PGLANGUAGE_H
#define PGLANGUAGE_H

#include "schema/pgobject.h"

class PGLanguage : public PGObject
{
public:
	Q_OBJECT
public:
	PGLanguage(const PGConnection *connection, const QString &name);
	PGLanguage(const PGConnection *connection);

	virtual bool exists() const;
	virtual void appendOrRefreshObject(PGObject *object = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
};

#endif // PGLANGUAGE_H
