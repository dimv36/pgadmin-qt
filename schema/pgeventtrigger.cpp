#include "schema/pgeventtrigger.h"

PGEventTrigger::PGEventTrigger(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_EVENTTRIGGER, name, QIcon(":/trigger"))
{
	if (_objectProperties.boolValue("enabled"))
		setIcon(ColumnText, QIcon(":/trigger"));
	else
		setIcon(ColumnText, QIcon(":/trigger-disabled"));
}

PGEventTrigger::PGEventTrigger(const PGConnection *connection)
: PGObject(connection, COLLECTION_EVENTTRIGGERS, QObject::tr("Event triggers"), QIcon(":/triggers"))
{}

bool PGEventTrigger::exists() const
{
	return true;
}

void PGEventTrigger::appendOrRefreshObject(PGObject *object)
{
	PGEventTrigger *evtrigger = nullptr;

	QString query = "SELECT e.oid, e.xmin, e.evtname AS name, \n"
					"       replace(e.evtevent, '_', ' ') AS eventname, \n"
					"       pg_catalog.pg_get_userbyid(e.evtowner) AS eventowner, \n"
					"       CASE e.evtenabled WHEN 'O' THEN 'enabled' WHEN 'R' THEN 'replica' WHEN 'A' THEN 'always' WHEN 'D' THEN 'disabled' END AS enabled, \n"
					"       e.evtfoid AS eventfuncoid, e.evtfoid::regproc AS eventfunname, \n"
					"       array_to_string(array(SELECT quote_literal(x) from unnest(evttags) as t(x)), ', ') AS when, \n"
					"       pg_catalog.obj_description(e.oid, 'pg_event_trigger') AS comment, \n"
					"       p.prosrc AS source, p.pronamespace AS schemaoid, l.lanname AS language, \n"
					"       (SELECT array_agg(label) FROM pg_seclabel sl1 WHERE sl1.objoid = e.oid) AS labels, \n"
					"       (SELECT array_agg(provider) FROM pg_seclabel sl2 WHERE sl2.objoid = e.oid) AS providers \n"
					"FROM pg_event_trigger e \n"
					"LEFT OUTER JOIN pg_proc p ON p.oid = e.evtfoid \n"
					"LEFT OUTER JOIN pg_language l ON l.oid = p.prolang \n"
					"%1 \n"
					"ORDER BY e.evtname";
	query = query.arg(object ? QString("WHERE e.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("name");

			if (!object)
				evtrigger = new PGEventTrigger(_connection, name);
			else
				evtrigger = dynamic_cast<PGEventTrigger *>(object);

			evtrigger->setObjectAttribute("oid", set->oidValue("oid"));
			evtrigger->setObjectAttribute("xmin", set->oidValue("xmin"));
			evtrigger->setObjectAttribute("owner", set->value("eventowner"));
			evtrigger->setObjectAttribute("funcoid", set->oidValue("eventfuncoid"));
			evtrigger->setObjectAttribute("language", set->value("language"));
			evtrigger->setObjectAttribute("source", set->value("source"));
			evtrigger->setObjectAttribute("event", set->value("eventname"));
			evtrigger->setObjectAttribute("funcname", set->value("eventfunname"));
			QString status = set->value("enabled");
			evtrigger->setObjectAttribute("status", status);
			evtrigger->setObjectAttribute("enabled", (status == "enabled" ? true : false));
			evtrigger->setObjectAttribute("comment", set->value("comment"));
			evtrigger->setObjectAttribute("when", set->value("when"));
			evtrigger->setObjectAttribute("schemaoid", set->oidValue("schemaoid"));
			evtrigger->parseSecurityLabels(set->value("providers"), set->value("labels"));

			if (!object)
				addChild(evtrigger);

			set->moveNext();
		}

		delete set;
	}
}

void PGEventTrigger::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Event"), _objectProperties.stringValue("event"));
	tab->addRow(QObject::tr("Function"), _objectProperties.stringValue("funcname"));
	tab->addRow(QObject::tr("When?"), _objectProperties.stringValue("when"));
	tab->addRow(QObject::tr("Enabled?"), _objectProperties.boolValue("enabled"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	appendSecurityLabels(tab);
}
