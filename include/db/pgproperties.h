#ifndef PGPROPERTIES_H
#define PGPROPERTIES_H

#include <QMap>
#include <QVariant>
#include "includes.h"

class PGKeyValueSetting : public QPair<QString, QString>
{
public:
	PGKeyValueSetting();
	PGKeyValueSetting(const QString &first, const QString &second);
};

class PGKeyValueSettings : public QVector<PGKeyValueSetting>
{
public:
	PGKeyValueSettings();
};
Q_DECLARE_METATYPE(PGKeyValueSettings)

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

	PGKeyValueSettings seclabels() const;
	PGKeyValueSettings variables() const;

	int intValue(const QString &key) const;
	Oid oidValue(const QString &key) const;
	QString stringValue(const QString &key) const;
	bool boolValue(const QString &key) const;
	QString dateTimeStringValue(const QString &key) const;
};

#endif // PGPROPERTIES_H
