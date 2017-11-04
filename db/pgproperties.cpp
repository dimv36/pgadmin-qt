#include <QDebug>
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


