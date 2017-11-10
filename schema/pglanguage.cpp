#include "schema/pglanguage.h"

PGLanguage::PGLanguage(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_LANGUAGE, name, QIcon(":/language"))
{}

PGLanguage::PGLanguage(const PGConnection *connection)
: PGObject(connection, COLLECTION_LANGUAGES, QObject::tr("Languages"), QIcon(":/languages"))
{}

bool PGLanguage::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_language WHERE oid = %1)")
										  .arg(_objectProperties.oidString()));
}

void PGLanguage::appendOrRefreshObject(PGObject *object)
{
	PGLanguage *language = nullptr;

	QString query = "SELECT lan.oid, lanname, lanpltrusted, lanacl, hp.proname AS lanproc, vp.proname AS lanval, description, \n"
					"       pg_get_userbyid(lan.lanowner) AS owner, \n"
					"       ip.proname AS laninl, \n"
					"       (SELECT array_agg(label) FROM pg_seclabels sl1 WHERE sl1.objoid = lan.oid) AS labels\n, "
					"       (SELECT array_agg(provider) FROM pg_seclabels sl2 WHERE sl2.objoid = lan.oid) AS providers\n "
					"FROM pg_language lan\n "
					"  JOIN pg_proc hp on hp.oid = lanplcallfoid\n "
					"  LEFT OUTER JOIN pg_proc ip on ip.oid = laninline\n "
					"  LEFT OUTER JOIN pg_proc vp on vp.oid = lanvalidator\n "
					"  LEFT OUTER JOIN pg_description des ON (des.objoid = lan.oid AND des.objsubid = 0 AND des.classoid = 'pg_language'::regclass)\n "
					"WHERE lanispl IS TRUE\n "
					"%1\n "
					"ORDER BY lanname";
	query = query.arg(object ? QString("AND lan.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("lanname");

			if (!object)
				language = new PGLanguage(_connection, name);
			else
				language = dynamic_cast<PGLanguage *>(object);

			language->setObjectAttribute("oid", set->oidValue("oid"));
			language->setObjectAttribute("owner", set->value("owner"));
			language->setObjectAttribute("acl", set->value("lanacl"));
			language->setObjectAttribute("comment", set->value("description"));
			language->setObjectAttribute("handlerproc", set->value("lanproc"));
			language->setObjectAttribute("inlineproc", set->value("laninl"));
			language->setObjectAttribute("validatorproc", set->value("lanval"));
			language->setObjectAttribute("trusted", set->value("lanpltrusted"));
			language->parseSecurityLabels(set->value("providers"), set->value("labels"));

			if (!object)
				addChild(language);

			set->moveNext();
		}
		delete set;
	}
}

void PGLanguage::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Trusted?"), _objectProperties.boolValue("trusted"));
	tab->addRow(QObject::tr("Handler function"), _objectProperties.stringValue("handlerproc"));
	tab->addRow(QObject::tr("Inline function"), _objectProperties.stringValue("inlineproc"));
	tab->addRow(QObject::tr("Validator function"), _objectProperties.stringValue("validatorproc"));
	tab->addRow(QObject::tr("System language?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	appendSecurityLabels(tab);
}
