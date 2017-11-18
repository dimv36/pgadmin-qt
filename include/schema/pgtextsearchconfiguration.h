#ifndef PGTEXTSEARCHCONFIGURATION_H
#define PGTEXTSEARCHCONFIGURATION_H

#include "schema/pgobject.h"

class PGTextSearchConfiguration : public PGObject
{
	Q_OBJECT
public:
	PGTextSearchConfiguration(const PGConnection *connection, const QString &name);
	PGTextSearchConfiguration(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

private:
	Oid _schemaOid;
	QString _schema;
};

#endif // PGTEXTSEARCHCONFIGURATION_H
