#include "schema/pgtextsearchparser.h"

PGTextSearchParser::PGTextSearchParser(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TEXTSEARCHPARSER, name, QIcon(":/ftsparser"))
{}

PGTextSearchParser::PGTextSearchParser(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_TEXTSEARCHPARSERS, QObject::tr("FTS Parsers"), QIcon(":/ftsparsers")),
  _schemaOid(schemaOid)
{}

void PGTextSearchParser::appendOrRefreshObject(PGObject *object)
{
	PGTextSearchParser *parser = nullptr;

	QString query = "SELECT prs.oid, prs.prsname, prs.prsstart, prs.prstoken, \n"
					"       prs.prsend, prs.prslextype, prs.prsheadline, description \n"
					"FROM pg_ts_parser prs \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = prs.oid AND des.classoid = 'pg_ts_parser'::regclass) \n"
					"WHERE prs.prsnamespace = %1 \n"
					"%2 \n"
					"ORDER BY prs.prsname";

	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND prs.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("prsname");

			if (!object)
				parser = new PGTextSearchParser(_connection, name);
			else
				parser = dynamic_cast<PGTextSearchParser *>(object);

			parser->setObjectAttribute("oid", set->oidValue("oid"));
			parser->setObjectAttribute("comment", set->value("description"));
			parser->setObjectAttribute("startproc", set->value("prsstart"));
			parser->setObjectAttribute("tokenproc", set->value("prstoken"));
			parser->setObjectAttribute("endproc", set->value("prsend"));
			parser->setObjectAttribute("lextype", set->value("prslextype"));
			QString headline = set->value("prsheadline");
			parser->setObjectAttribute("headlineproc", headline != "-" ? headline : "");

			if (!object)
				addChild(parser);

			set->moveNext();
		}

		delete set;
	}
}

void PGTextSearchParser::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Start"), _objectProperties.stringValue("startproc"));
	tab->addRow(QObject::tr("Gettoken"), _objectProperties.stringValue("tokenproc"));
	tab->addRow(QObject::tr("End"), _objectProperties.stringValue("endproc"));
	tab->addRow(QObject::tr("Lextypes"), _objectProperties.stringValue("lextype"));
	tab->addRow(QObject::tr("Headline"), _objectProperties.stringValue("headlineproc"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGTextSearchParser::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_ts_parser WHERE oid = %1)")
										  .arg(oidString()));
}
