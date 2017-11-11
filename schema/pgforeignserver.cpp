#include "schema/pgforeignserver.h"
#include "schema/pgusermapping.h"

PGForeignServer::PGForeignServer(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_FOREIGNSERVER, name, QIcon(":/foreignserver"))
{}

PGForeignServer::PGForeignServer(const PGConnection *connection, const Oid fdwOid)
: PGObject(connection, COLLECTION_FOREIGHNSERVERS, QObject::tr("Foreign Servers"), QIcon(":/foreignservers")),
  _fdwOid(fdwOid)
{}

void PGForeignServer::appendCollectionItems()
{
	addChild(newPGObject<PGUserMapping>(_connection, _objectProperties.oid()));
}

void PGForeignServer::appendOrRefreshObject(PGObject *object)
{
	PGForeignServer *foreignServer = nullptr;

	QString query = "SELECT srv.oid, srvname, srvtype, srvversion, srvacl, fdw.fdwname as fdwname, description, \n"
					"       array_to_string(srvoptions, ', ') AS srvoptions, \n"
					"       pg_get_userbyid(srvowner) AS srvowner \n"
					"FROM pg_foreign_server srv \n"
					"LEFT OUTER JOIN pg_foreign_data_wrapper fdw on fdw.oid = srvfdw \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = srv.oid AND des.objsubid = 0 AND des.classoid = 'pg_foreign_server'::regclass) \n"
					"WHERE srvfdw = %1 \n"
					"%2 \n"
					"ORDER BY srvname";

	query = query.arg(QString::number(_fdwOid),
					  object ? QString("AND srv.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);

	if (set)
	{
		while (!set->eof())
		{
			QString srvname = set->value("srvname");

			if (!object)
				foreignServer = new PGForeignServer(_connection, srvname);
			else
				foreignServer = dynamic_cast<PGForeignServer *>(object);

			foreignServer->setObjectAttribute("oid", set->oidValue("oid"));
			foreignServer->setObjectAttribute("owner", set->value("srvowner"));
			foreignServer->setObjectAttribute("acl", set->value("srvacl"));
			foreignServer->setObjectAttribute("comment", set->value("description"));
			foreignServer->setObjectAttribute("type", set->value("srvtype"));
			foreignServer->setObjectAttribute("version", set->value("srvversion"));
			foreignServer->setObjectAttribute("options", set->value("srvoptions"));

			foreignServer->afterConstruction();

			if (!object)
				addChild(foreignServer);

			set->moveNext();
		}

		delete set;
	}
}

void PGForeignServer::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Type"), _objectProperties.stringValue("type"));
	tab->addRow(QObject::tr("Options"), _objectProperties.stringValue("options"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGForeignServer::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_foreign_server WHERE oid = %1)")
										  .arg(oidString()));
}
