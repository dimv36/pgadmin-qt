#ifndef PGTEXTSEARCTEMPLATE_H
#define PGTEXTSEARCTEMPLATE_H

#include "schema/pgobject.h"

class PGTextSearchTemplate : public PGObject
{
	Q_OBJECT
public:
	PGTextSearchTemplate(const PGConnection *connection, const QString &name);
	PGTextSearchTemplate(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
};

#endif // PGTEXTSEARCTEMPLATE_H
