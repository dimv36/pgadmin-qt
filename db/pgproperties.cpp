#include <QDebug>
#include <QString>
#include <QDateTime>
#include "db/pgproperties.h"

PGKeyValueSetting::PGKeyValueSetting()
: QPair<QString, QString>()
{}

PGKeyValueSetting::PGKeyValueSetting(const QString &key, const QString &value)
: QPair<QString, QString>(key, value)
{}

PGKeyValueSettings::PGKeyValueSettings()
: QVector<PGKeyValueSetting>()
{}


/************************************************************************************* */

PGProperties::PGProperties()
{}

Oid PGProperties::oid() const
{
	return oidValue("oid");
}

QString PGProperties::oidString() const
{
	return stringValue("oid");
}

void PGProperties::setOid(const Oid oid)
{
	operator []("oid") = oid;
}

QString PGProperties::name() const
{
	return stringValue("name");
}

void PGProperties::setName(const QString &name)
{
	operator []("name") = name;
}

QString PGProperties::owner() const
{
	return stringValue("owner");
}

void PGProperties::setOwner(const QString &owner)
{
	operator []("owner") = owner;
}

QString PGProperties::comment() const
{
	return stringValue("comment");
}

void PGProperties::setComment(const QString &comment)
{
	operator []("comment") = comment;
}

QString PGProperties::acl() const
{
	return stringValue("acl");
}

void PGProperties::setAcl(const QString &acl)
{
	operator []("acl") = acl;
}

PGKeyValueSettings PGProperties::seclabels() const
{
	QVariant variant = operator [] ("seclabels");
	return qvariant_cast<PGKeyValueSettings>(variant);
}

PGKeyValueSettings PGProperties::variables() const
{
	QVariant variant = operator [] ("variables");
	return qvariant_cast<PGKeyValueSettings>(variant);
}

int PGProperties::intValue(const QString &key) const
{
	if (contains(key))
		return value(key).toInt();
	return 0;
}

Oid PGProperties::oidValue(const QString &key) const
{
	if (contains(key))
		return value(key).toInt();
	return InvalidOid;
}

QString PGProperties::stringValue(const QString &key) const
{
	if (contains(key))
		return value(key).toString();
	return QString();
}

bool PGProperties::boolValue(const QString &key) const
{
	if (contains(key))
		return value(key).toBool();
	return false;
}

QString PGProperties::dateTimeStringValue(const QString &key) const
{
	if (contains(key))
		return value(key).toDateTime().toString(Qt::LocalDate);
	return QString();
}
