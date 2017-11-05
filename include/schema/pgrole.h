#ifndef PGROLE_H
#define PGROLE_H

#include "pgobject.h"

class PGRole : public PGObject
{
public:
	PGRole(const PGConnection *connection, const ObjectType objectType,
		   const QString &name, const QIcon &icon = QIcon());
	PGRole(PGConnection *connection, const ObjectType objectType,
		   const QString &name, const QIcon &collectionIcon, const QIcon &objectIcon, bool canLogin);

	virtual void showSingleObjectProperties(PropertyTable *);

private:
	void getConfigurationVariables();

protected:
	bool _canlogin;
};

#endif // PGROLE_H
