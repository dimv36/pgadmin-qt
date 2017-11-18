#include <QDebug>
#include "schema/pgschema.h"
#include "schema/pgcollation.h"
#include "schema/pgconversion.h"
#include "schema/pgtextsearchconfiguration.h"
#include "schema/pgtextsearchdictionary.h"
#include "schema/pgtextsearchparser.h"

PGSchema::PGSchema(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_SCHEMA, name, QIcon(":/schema"))
{}

PGSchema::PGSchema(const PGConnection *connection)
: PGObject(connection, COLLECTION_SCHEMAS, QObject::tr("Schemas"), QIcon(":/schemas"))
{}

void PGSchema::appendCollectionItems()
{
	Oid schemaOid = _objectProperties.oid();
	addChild(newPGObject<PGCollation>(_connection, schemaOid));
	addChild(newPGObject<PGConversion>(_connection, schemaOid));
	addChild(newPGObject<PGTextSearchConfiguration>(_connection, schemaOid));
	addChild(newPGObject<PGTextSearchDictionary>(_connection, schemaOid));
	addChild(newPGObject<PGTextSearchParser>(_connection, schemaOid));
}

void PGSchema::appendOrRefreshObject(PGObject *object)
{
	PGSchema *schema = nullptr;

	QString query = "SELECT nsp.oid AS oid, nsp.nspname AS nspname, pg_get_userbyid(nspowner) AS owner, \n"
					"       nspacl, description, has_schema_privilege(nsp.oid, 'CREATE') AS cancreate, \n"
					"       (SELECT array_agg(label) FROM pg_seclabels sl1 WHERE sl1.objoid = nsp.oid) AS labels, \n"
					"       (SELECT array_agg(provider) FROM pg_seclabels sl2 WHERE sl2.objoid = nsp.oid) AS providers \n"
					"FROM pg_namespace nsp \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = nsp.oid AND des.classoid = 'pg_namespace'::regclass) \n"
					"%1";
	query = query.arg(object ? QString("WHERE nsp.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString nspname = set->value("nspname");

			if (!object)
				schema = new PGSchema(_connection, nspname);
			else
				schema = dynamic_cast<PGSchema *>(object);

			schema->setObjectAttribute("oid", set->oidValue("oid"));
			schema->setObjectAttribute("acl", set->value("nspacl"));
			schema->setObjectAttribute("owner", set->value("owner"));
			schema->setObjectAttribute("cancreate", set->boolValue("cancreate"));
			schema->setObjectAttribute("comment", set->value("description"));
			schema->getDefaultPrivileges();
			schema->parseSecurityLabels(set->value("providers"), set->value("labels"));

			schema->afterConstruction();

			if (!object)
				addChild(schema);

			set->moveNext();
		}
		delete set;
	}
}

void PGSchema::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Default table ACL"), _objectProperties.stringValue("defaclontables"));
	tab->addRow(QObject::tr("Default sequence ACL"), _objectProperties.stringValue("defaclonsequences"));
	tab->addRow(QObject::tr("Default function ACL"), _objectProperties.stringValue("defaclonfunctions"));
	tab->addRow(QObject::tr("Default type ACL"), _objectProperties.stringValue("defaclontypes"));
	tab->addRow(QObject::tr("Is system schema?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
	appendSecurityLabels(tab);
}

bool PGSchema::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_namespace WHERE oid = %1)")
										  .arg(_objectProperties.oidString()));
}

void PGSchema::getDefaultPrivileges()
{
	setObjectAttribute("defaclontables",
					   _connection->executeScalar(QString("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = %1 AND defaclobjtype='r'")
												  .arg(_objectProperties.oidString())));
	setObjectAttribute("defaclonsequences",
					   _connection->executeScalar(QString("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = %1 AND defaclobjtype='S'")
												  .arg(_objectProperties.oidString())));
	setObjectAttribute("defaclonfuncs",
					   _connection->executeScalar(QString("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = %1 AND defaclobjtype='f'")
												  .arg(_objectProperties.oidString())));
	setObjectAttribute("defaclontypes",
					   _connection->executeScalar(QString("SELECT defaclacl FROM pg_catalog.pg_default_acl dacl WHERE dacl.defaclnamespace = %1 AND defaclobjtype='T'")
												  .arg(_objectProperties.oidString())));
}
