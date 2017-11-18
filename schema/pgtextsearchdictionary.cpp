#include "schema/pgtextsearchdictionary.h"

PGTextSearchDictionary::PGTextSearchDictionary(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TEXTSEARCHDICTIONARY, name, QIcon(":/ftsdictionary"))
{}

PGTextSearchDictionary::PGTextSearchDictionary(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_TEXTSEARCHDICTIONARY, QObject::tr("FTS Dictionaries"), QIcon(":/ftsdictionaries")),
  _schemaOid(schemaOid)
{}

void PGTextSearchDictionary::appendOrRefreshObject(PGObject *object)
{
	PGTextSearchDictionary *dict = nullptr;

	QString query = "SELECT dict.oid, dict.dictname, pg_get_userbyid(dict.dictowner) AS dictowner, \n"
					"       t.tmplname, dict.dictinitoption, description \n"
					"FROM pg_ts_dict dict \n"
					"LEFT OUTER JOIN pg_ts_template t ON t.oid = dict.dicttemplate\n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = dict.oid AND des.classoid = 'pg_ts_dict'::regclass) \n"
					"WHERE dict.dictnamespace = %1"
					"%2 \n"
					"ORDER BY dict.dictname";

	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND dict.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("dictname");

			if (!object)
				dict = new PGTextSearchDictionary(_connection, name);
			else
				dict = dynamic_cast<PGTextSearchDictionary *>(object);

			dict->setObjectAttribute("oid", set->oidValue("oid"));
			dict->setObjectAttribute("owner", set->value("dictowner"));
			dict->setObjectAttribute("comment", set->value("description"));
			dict->setObjectAttribute("template", set->value("tmplname"));
			dict->setObjectAttribute("options", set->value("dictinitoption"));

			if (!object)
				addChild(dict);

			set->moveNext();
		}

		delete set;
	}
}

void PGTextSearchDictionary::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Template"), _objectProperties.stringValue("template"));
	tab->addRow(QObject::tr("Options"), _objectProperties.stringValue("options"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGTextSearchDictionary::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_ts_dict WHERE oid = %1)")
										  .arg(oidString()));
}
