#include "schema/pgconversion.h"

PGConversion::PGConversion(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_CONVERSION, name, QIcon(":/conversion"))
{}

PGConversion::PGConversion(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_CONVERSIONS, QObject::tr("Conversions"), QIcon(":/conversions")),
  _schemaOid(schemaOid)
{}

void PGConversion::appendOrRefreshObject(PGObject *object)
{
	PGConversion *conversion = nullptr;

	QString query = "SELECT co.oid, co.*, \n"
					"       pg_encoding_to_char(conforencoding) AS forencoding, \n"
					"       pg_get_userbyid(conowner) AS owner, \n"
					"       pg_encoding_to_char(contoencoding) AS toencoding, \n"
					"       proname, nspname, description \n"
					"FROM pg_conversion co \n"
					"JOIN pg_proc pr ON pr.oid = conproc \n"
					"JOIN pg_namespace na ON na.oid = pr.pronamespace \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = co.oid AND des.objsubid = 0 AND des.classoid = 'pg_conversion'::regclass) \n"
					"WHERE connamespace = %1 \n"
					"%2 \n"
					"ORDER BY conname";
	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND co.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString conname = set->value("conname");

			if (!object)
				conversion = new PGConversion(_connection, conname);
			else
				conversion = dynamic_cast<PGConversion *>(object);

			conversion->setObjectAttribute("oid", set->oidValue("oid"));
			conversion->setObjectAttribute("owner", set->value("owner"));
			conversion->setObjectAttribute("comment", set->value("description"));
			conversion->setObjectAttribute("fromencoding", set->value("forencoding"));
			conversion->setObjectAttribute("toencoding", set->value("toencoding"));
			conversion->setObjectAttribute("proname", set->value("proname"));
			conversion->setObjectAttribute("procnamespace", set->value("nspname"));
			conversion->setObjectAttribute("condefault", set->boolValue("condefault"));

			if (!object)
				addChild(conversion);

			set->moveNext();
		}

		delete set;
	}
}

void PGConversion::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("From"), _objectProperties.stringValue("fromencoding"));
	tab->addRow(QObject::tr("To"), _objectProperties.stringValue("toencoding"));
	tab->addRow(QObject::tr("Function"), fullConversionFunctionName());
	tab->addRow(QObject::tr("Default?"), _objectProperties.boolValue("condefault"));
	tab->addRow(QObject::tr("System conversion?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGConversion::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_conversion WHERE oid = %1)")
										  .arg(oidString()));
}

QString PGConversion::fullConversionFunctionName() const
{
	return QString("%1.%2").arg(_objectProperties.stringValue("procnamespace"), _objectProperties.stringValue("proname"));
}
