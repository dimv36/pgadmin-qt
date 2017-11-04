#ifndef PGPROPERTIES_H
#define PGPROPERTIES_H

#include <QMap>
#include <QVariant>
#include "includes.h"

class PGSecurityLabel : public QPair<QString, QString>
{
public:
	PGSecurityLabel();
	PGSecurityLabel(const QString &provider, const QString &label);

	QString provider() const;
	void setProvider(const QString &provider);

	QString label() const;
	void setLabel(const QString &label);
};

class PGSecurityLabels : public QVector<PGSecurityLabel>
{
public:
	PGSecurityLabels();
};
Q_DECLARE_METATYPE(PGSecurityLabels)

class PGProperties : public QMap<QString, QVariant>
{
public:
	PGProperties();

	Oid oid() const;
	void setOid(const Oid oid);

	QString name() const;
	void setName(const QString &name);

	QString owner() const;
	void setOwner(const QString &owner);

	QString comment() const;
	void setComment(const QString &comment);

	QString acl() const;
	void setAcl(const QString &acl);

	PGSecurityLabels seclabels() const;

	int intValue(const QString &key) const;
	Oid oidValue(const QString &key) const;
	QString stringValue(const QString &key) const;
	bool boolValue(const QString &key) const;
};

#endif // PGPROPERTIES_H
