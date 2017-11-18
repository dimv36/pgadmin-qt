#include "schema/pgtextsearchtemplate.h"

PGTextSearchTemplate::PGTextSearchTemplate(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TEXTSEARCHTEMPLATE, name, QIcon(":/ftstemplate"))
{}

PGTextSearchTemplate::PGTextSearchTemplate(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_TEXTSEARCHTEMPLATES, QObject::tr("FTS Templates"), QIcon(":/ftstemplates")),
  _schemaOid(schemaOid)
{}

void PGTextSearchTemplate::appendOrRefreshObject(PGObject *object)
{
	PGTextSearchTemplate *ftstemplate = nullptr;

	QString query = "SELECT tmpl.oid, tmpl.tmplname, tmpl.tmplinit, tmpl.tmpllexize, description \n"
					"FROM pg_ts_template tmpl \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = tmpl.oid AND des.classoid = 'pg_ts_template'::regclass) \n"
					"WHERE tmpl.tmplnamespace = %1 \n"
					"%2 \n"
					"ORDER BY tmpl.tmplname";

	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND tmpl.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("tmplname");

			if (!object)
				ftstemplate = new PGTextSearchTemplate(_connection, name);
			else
				ftstemplate = dynamic_cast<PGTextSearchTemplate *>(object);

			ftstemplate->setObjectAttribute("oid", set->oidValue("oid"));
			ftstemplate->setObjectAttribute("comment", set->value("description"));
			QString init = set->value("tmplinit");
			ftstemplate->setObjectAttribute("initproc", init != "-" ? init : "");
			ftstemplate->setObjectAttribute("lexizeproc", set->value("tmpllexize"));

			if (!object)
				addChild(ftstemplate);
			set->moveNext();
		}

		delete set;
	}
}

void PGTextSearchTemplate::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Init"), _objectProperties.stringValue("initproc"));
	tab->addRow(QObject::tr("Lexize"), _objectProperties.stringValue("lexizeproc"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGTextSearchTemplate::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_ts_template WHERE oid = %1)")
										  .arg(oidString()));
}
