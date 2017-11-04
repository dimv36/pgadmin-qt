#include <QDebug>
#include <QString>
#include "db/pgproperties.h"

PGProperties::PGProperties()
{}

Oid PGProperties::oid() const
{
	return oidValue("oid");
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

PGSecurityLabels PGProperties::seclabels() const
{
	QVariant variant = operator [] ("seclabels");
	return qvariant_cast<PGSecurityLabels>(variant);
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

PGSecurityLabel::PGSecurityLabel()
: QPair<QString, QString>()
{}

PGSecurityLabel::PGSecurityLabel(const QString &provider, const QString &label)
: QPair<QString, QString>(provider, label)
{}

QString PGSecurityLabel::provider() const
{
	return first;
}

void PGSecurityLabel::setProvider(const QString &provider)
{
	first = provider;
}

QString PGSecurityLabel::label() const
{
	return second;
}

void PGSecurityLabel::setLabel(const QString &label)
{
	second = label;
}

PGSecurityLabels::PGSecurityLabels()
: QVector<PGSecurityLabel>()
{}
