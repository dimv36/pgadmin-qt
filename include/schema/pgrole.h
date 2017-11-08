#ifndef PGROLE_H
#define PGROLE_H

#include "pgobject.h"

class PGRole : public PGObject
{
public:
	PGRole(const PGConnection *connection, const ObjectType objectType,
		   const QString &name, const QIcon &icon = QIcon());
	PGRole(const PGConnection *connection, const ObjectType objectType,
		   const QString &name, const QIcon &collectionIcon, bool canLogin);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	void getConfigurationVariables();

protected:
	bool _canlogin;
};

#endif // PGROLE_H
