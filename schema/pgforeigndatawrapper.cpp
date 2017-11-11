#include "schema/pgforeigndatawrapper.h"
#include "schema/pgforeignserver.h"

PGForeignDataWrapper::PGForeignDataWrapper(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_FDW, name, QIcon(":/foreigndatawrapper"))
{}

PGForeignDataWrapper::PGForeignDataWrapper(const PGConnection *connection)
: PGObject(connection, COLLECTION_FDWS, QObject::tr("Foreign Data Wrappers"), QIcon(":/foreigndatawrappers"))
{}

void PGForeignDataWrapper::appendCollectionItems()
{
	addChild(newPGObject<PGForeignServer>(_connection, _objectProperties.oid()));
}

void PGForeignDataWrapper::appendOrRefreshObject(PGObject *object)
{
	PGForeignDataWrapper *fdw = nullptr;

	QString query = "SELECT fdw.oid, fdwname, fdwhandler, fdwvalidator, fdwacl, \n"
					"       vh.proname AS fdwhan, vp.proname AS fdwval, description, \n"
					"       array_to_string(fdwoptions, ',') AS fdwoptions, \n"
					"       pg_get_userbyid(fdwowner) AS owner\n "
					"FROM pg_foreign_data_wrapper fdw\n "
					"LEFT OUTER JOIN pg_proc vh on vh.oid = fdwhandler \n"
					"LEFT OUTER JOIN pg_proc vp on vp.oid = fdwvalidator \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = fdw.oid AND des.objsubid = 0 AND des.classoid='pg_foreign_data_wrapper'::regclass) \n"
					"%1 \n"
					"ORDER BY fdwname";
	query = query.arg(object ? QString("WHERE fdw.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("fdwname");

			if (!object)
				fdw = new PGForeignDataWrapper(_connection, name);
			else
				fdw = dynamic_cast<PGForeignDataWrapper *>(object);

			fdw->setObjectAttribute("oid", set->oidValue("oid"));
			fdw->setObjectAttribute("owner", set->value("owner"));
			fdw->setObjectAttribute("acl", set->value("fdwacl"));
			fdw->setObjectAttribute("handlerproc", set->value("fdwhan"));
			fdw->setObjectAttribute("validatorproc", set->value("fdwval"));
			fdw->setObjectAttribute("options", set->value("fdwoptions"));
			fdw->setObjectAttribute("comment", set->value("description"));

			fdw->afterConstruction();

			if (!object)
				addChild(fdw);

			set->moveNext();
		}
		delete set;
	}
}

void PGForeignDataWrapper::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("ACL"), _objectProperties.acl());
	tab->addRow(QObject::tr("Handler"), _objectProperties.stringValue("handlerproc"));
	tab->addRow(QObject::tr("Validator"), _objectProperties.stringValue("validatorproc"));
	tab->addRow(QObject::tr("Options"), _objectProperties.stringValue("options"));
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGForeignDataWrapper::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_foreign_data_wrapper WHERE oid = %1)")
										  .arg(oidString()));
}
