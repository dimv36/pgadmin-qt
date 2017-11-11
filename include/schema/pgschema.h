#ifndef PGSHEMA_H
#define PGSHEMA_H

#include "schema/pgobject.h"

class PGSchema : public PGObject
{
	Q_OBJECT
public:
	PGSchema(const PGConnection *connection, const QString &name);
	PGSchema(const PGConnection *connection);

	virtual void appendCollectionItems();
	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

protected:
	void getDefaultPrivileges();
};

#endif // PGSHEMA_H
