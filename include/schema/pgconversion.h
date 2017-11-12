#ifndef PGCONVERSION_H
#define PGCONVERSION_H

#include "schema/pgobject.h"

class PGConversion : public PGObject
{
	Q_OBJECT
public:
	PGConversion(const PGConnection *connection, const QString &name);
	PGConversion(const PGConnection *connection, const Oid schemaOid);

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

	QString fullConversionFunctionName() const;

private:
	Oid _schemaOid;
	QString _schema;
};

#endif // PGCONVERSION_H
