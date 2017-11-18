#ifndef PGTEXTSEARCHPARSER_H
#define PGTEXTSEARCHPARSER_H

#include "schema/pgobject.h"

class PGTextSearchParser : public PGObject
{
	Q_OBJECT
public:
	PGTextSearchParser(const PGConnection *connection, const QString &name);
	PGTextSearchParser(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
};

#endif // PGTEXTSEARCHPARSER_H
