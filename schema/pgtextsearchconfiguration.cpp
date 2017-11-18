#include "schema/pgtextsearchconfiguration.h"

PGTextSearchConfiguration::PGTextSearchConfiguration(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_TEXTSEARCHCONFIGURATION, name, QIcon(":/ftsconfiguration"))
{}

PGTextSearchConfiguration::PGTextSearchConfiguration(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_TEXTSEARCHCONFIGURATION, QObject::tr("FTS Configurations"), QIcon(":/ftsconfigurations")),
  _schemaOid(schemaOid)
{}

void PGTextSearchConfiguration::appendOrRefreshObject(PGObject *object)
{
	PGTextSearchConfiguration *config = nullptr;

	QString query = "SELECT cfg.oid, cfg.cfgname, pg_get_userbyid(cfg.cfgowner) AS cfgowner, \n"
					"       cfg.cfgparser, parser.prsname AS parsername, description \n"
					"FROM pg_ts_config cfg \n"
					"LEFT OUTER JOIN pg_ts_parser parser ON parser.oid = cfg.cfgparser \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = cfg.oid AND des.classoid = 'pg_ts_config'::regclass) \n"
					"WHERE cfg.cfgnamespace = %1 \n"
					"%2 \n"
					"ORDER BY cfg.cfgname";
	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND cfg.oid = %1").arg(object->oidString()) : QString());

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("cfgname");

			if (!object)
				config = new PGTextSearchConfiguration(_connection, name);
			else
				config = dynamic_cast<PGTextSearchConfiguration *>(object);

			config->setObjectAttribute("oid", set->oidValue("oid"));
			config->setObjectAttribute("owner", set->value("cfgowner"));
			config->setObjectAttribute("comment", set->value("description"));
			config->setObjectAttribute("parser", set->value("parsername"));
			config->setObjectAttribute("parserOid", set->oidValue("cfgparser"));

			if (!object)
				addChild(config);

			set->moveNext();
		}

		delete set;
	}
}

void PGTextSearchConfiguration::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Parser"), _objectProperties.stringValue("parser"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGTextSearchConfiguration::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_ts_config WHERE oid = %1)")
										  .arg(oidString()));
}
