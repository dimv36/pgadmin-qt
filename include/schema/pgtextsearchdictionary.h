#ifndef PGTEXTSEARCHDICTIONARY_H
#define PGTEXTSEARCHDICTIONARY_H

#include "schema/pgobject.h"

class PGTextSearchDictionary : public PGObject
{
	Q_OBJECT
public:
	PGTextSearchDictionary(const PGConnection *connection, const QString &name);
	PGTextSearchDictionary(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
};

#endif // PGTEXTSEARCHDICTIONARY_H
