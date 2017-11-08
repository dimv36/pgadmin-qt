#include <QString>
#include "schema/pgcast.h"


PGCast::PGCast(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_CAST, name, QIcon(":/cast.png"))
{}

PGCast::PGCast(const PGConnection *connection)
: PGObject(connection, COLLECTION_CASTS, QObject::tr("Casts"), QIcon(":/casts.png"))
{}

void PGCast::appendOrRefreshObject(PGObject *object)
{
	PGCast *cast = nullptr;

	QString query = "SELECT ca.oid AS oid, ca.*, format_type(st.oid, NULL) AS srctyp, format_type(tt.oid, tt.typtypmod) AS trgtyp, \n"
					"       ns.nspname AS srcnspname, nt.nspname AS trgnspname, \n"
					"       proname, np.nspname AS pronspname, description \n"
					"FROM pg_cast ca\n "
					"JOIN pg_type st ON st.oid = castsource \n"
					"JOIN pg_namespace ns ON ns.oid = st.typnamespace \n"
					"JOIN pg_type tt ON tt.oid = casttarget \n"
					"JOIN pg_namespace nt ON nt.oid = tt.typnamespace \n"
					"LEFT JOIN pg_proc pr ON pr.oid = castfunc \n"
					"LEFT JOIN pg_namespace np ON np.oid = pr.pronamespace \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = ca.oid AND des.objsubid = 0 AND des.classoid = 'pg_cast'::regclass) \n"
					"%1 \n"
					"ORDER BY st.typname, tt.typname";
	query = query.arg(object ? QString("WHERE ca.oid = %1").arg(object->oidObjectAttribute("oid")) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString castname = QString("%1->%2").arg(set->value("srctyp"), set->value("trgtyp"));

			if (!object)
				cast = new PGCast(_connection, castname);
			else
				cast = dynamic_cast<PGCast *>(object);

			cast->setObjectAttribute("oid", set->oidValue("oid"));
			cast->setObjectAttribute("srctyp", set->value("srctyp"));
			cast->setObjectAttribute("srcnspname", set->value("srcnspname"));
			cast->setObjectAttribute("castsource", set->oidValue("castsource"));
			cast->setObjectAttribute("trgtyp", set->value("trgtyp"));
			cast->setObjectAttribute("trgnspname", set->value("trgnspname"));
			cast->setObjectAttribute("casttarget", set->oidValue("casttarget"));
			cast->setObjectAttribute("proname", set->value("proname"));
			cast->setObjectAttribute("pronspname", set->value("pronspname"));
			cast->setObjectAttribute("comment", set->value("description"));

			QString ct = set->value("castcontext");
			cast->setObjectAttribute("context", ct == "i" ?
													"IMPLICIT" :
												ct == "a" ?
													"ASSIGNMENT" :
												"EXPLICIT");

			if (!object)
				addChild(cast);

			set->moveNext();
		}
		delete set;
	}
}

void PGCast::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Source type"), _objectProperties.stringValue("srctyp"));
	tab->addRow(QObject::tr("Target type"), _objectProperties.stringValue("trgtyp"));
	QString function = _objectProperties.stringValue("proname");
	if (function.isEmpty())
		tab->addRow(QObject::tr("Function"), QObject::tr("(binary compatible)"));
	else
		tab->addRow(QObject::tr("Function"), QString("%1 (%2)").arg(_objectProperties.stringValue("proname"),
																	_objectProperties.stringValue("srctyp")));
	tab->addRow(QObject::tr("Context"), _objectProperties.stringValue("context"));
	tab->addRow(QObject::tr("System cast?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGCast::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS(SELECT * FROM pg_cast WHERE oid = %1)")
										  .arg(_objectProperties.oidString()));
}
