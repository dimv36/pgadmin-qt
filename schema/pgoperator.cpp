#include "schema/pgoperator.h"

PGOperator::PGOperator(const PGConnection *connection, const QString &name)
: PGObject(connection, OBJECT_OPERATOR, name, QIcon(":/operator"))
{}

PGOperator::PGOperator(const PGConnection *connection, const Oid schemaOid)
: PGObject(connection, COLLECTION_OPERATORS, QObject::tr("Operators"), QIcon(":/operators")),
  _schemaOid(schemaOid)
{}

QString PGOperator::fullName() const
{
	QString lefttype = _objectProperties.stringValue("lefttype");
	QString righttype = _objectProperties.stringValue("righttype");

	if (lefttype.isEmpty() || righttype.isEmpty())
		return QString("%1 (%2%3)").arg(_objectProperties.name(), lefttype, righttype);
	else
		return QString("%1 (%2, %3)").arg(_objectProperties.name(), lefttype, righttype);
}

void PGOperator::appendOrRefreshObject(PGObject *object)
{
	PGOperator *op = nullptr;

	QString query = "SELECT op.oid, op.oprname, pg_get_userbyid(op.oprowner) AS opowner, \n"
					"       op.oprkind, op.oprcanhash, op.oprcanmerge, \n"
					"       op.oprleft, op.oprright, lt.typname AS lefttype, rt.typname AS righttype, \n"
					"       et.typname AS resulttype, \n"
					"       co.oprname AS compop, ne.oprname as negop, \n"
					"       op.oprcode AS operproc, op.oprjoin AS joinproc, op.oprrest AS restrproc, description \n"
					"FROM pg_operator op \n"
					"LEFT OUTER JOIN pg_type lt ON lt.oid = op.oprleft \n"
					"LEFT OUTER JOIN pg_type rt ON rt.oid = op.oprright \n"
					"JOIN pg_type et on et.oid = op.oprresult \n"
					"LEFT OUTER JOIN pg_operator co ON co.oid = op.oprcom \n"
					"LEFT OUTER JOIN pg_operator ne ON ne.oid = op.oprnegate \n"
					"LEFT OUTER JOIN pg_description des ON (des.objoid = op.oid AND des.classoid = 'pg_operator'::regclass) \n"
					"WHERE op.oprnamespace = %1"
					"%2 \n"
					"ORDER BY op.oprname";
	query = query.arg(QString::number(_schemaOid),
					  object ? QString("AND op.oid = %1").arg(object->oidString()) : "");

	PGSet *set = _connection->executeSet(query);
	if (set)
	{
		while (!set->eof())
		{
			QString name = set->value("oprname");

			if (!object)
				op = new PGOperator(_connection, name);
			else
				op = dynamic_cast<PGOperator *>(object);

			op->setObjectAttribute("oid", set->oidValue("oid"));
			op->setObjectAttribute("owner", set->value("opowner"));
			op->setObjectAttribute("comment", set->value("description"));
			op->setObjectAttribute("lefttype", set->value("lefttype"));
			op->setObjectAttribute("righttype", set->value("righttype"));
			op->setObjectAttribute("oprleftoid", set->oidValue("oprleft"));
			op->setObjectAttribute("oprrightoid", set->oidValue("oprright"));
			op->setObjectAttribute("resulttype", set->value("resulttype"));
			op->setObjectAttribute("operproc", set->value("operproc"));
			QString joinproc = set->value("joinproc");
			if (joinproc != "-")
				op->setObjectAttribute("joinproc", joinproc);
			QString restproc = set->value("restrproc");
			if (restproc != "-")
				op->setObjectAttribute("restproc", restproc);
			op->setObjectAttribute("comproc", set->value("compop"));
			op->setObjectAttribute("negproc", set->value("negop"));
			QString kind = set->value("oprkind");
			op->setObjectAttribute("kind", kind == "b" ? QObject::tr("infix") :
														 kind == "l" ? QObject::tr("prefix") :
														 kind == "r" ? QObject::tr("postfix") :
																	   QObject::tr("Unknown"));
			op->setObjectAttribute("canhash", set->boolValue("oprcanhash"));
			op->setObjectAttribute("canmerge", set->boolValue("oprcanmerge"));

			op->afterConstruction();

			if (!object)
				addChild(op);

			set->moveNext();
		}
		delete set;
	}
}

void PGOperator::showSingleObjectProperties(PropertyTable *tab)
{
	tab->addRow(QObject::tr("Name"), _objectProperties.name());
	tab->addRow(QObject::tr("OID"), _objectProperties.oid());
	tab->addRow(QObject::tr("Owner"), _objectProperties.owner());
	tab->addRow(QObject::tr("Kind"), _objectProperties.stringValue("kind"));
	QString lefttype = _objectProperties.stringValue("lefttype");
	if (!lefttype.isEmpty())
		tab->addRow(QObject::tr("Left type"), lefttype);
	QString righttype = _objectProperties.stringValue("righttype");
	if (!righttype.isEmpty())
		tab->addRow(QObject::tr("Rights type"), righttype);
	tab->addRow(QObject::tr("Result type"), _objectProperties.stringValue("resulttype"));
	tab->addRow(QObject::tr("Operator function"), _objectProperties.stringValue("operproc"));
	tab->addRow(QObject::tr("Commutator"), _objectProperties.stringValue("comproc"));
	tab->addRow(QObject::tr("Negator"), _objectProperties.stringValue("negproc"));
	if (_objectProperties.hasKey("joinproc"))
		tab->addRow(QObject::tr("Join function"), _objectProperties.stringValue("joinproc"));
	if (_objectProperties.hasKey("restproc"))
		tab->addRow(QObject::tr("Restrict function"), _objectProperties.stringValue("restproc"));
	tab->addRow(QObject::tr("Supports hash?"), _objectProperties.boolValue("canhash"));
	tab->addRow(QObject::tr("Supports merge?"), _objectProperties.boolValue("canmerge?"));
	tab->addRow(QObject::tr("Is system operator?"), isSystemObject());
	tab->addRow(QObject::tr("Comment"), _objectProperties.comment());
}

bool PGOperator::exists() const
{
	return _connection->executeScalarBool(QString("SELECT EXISTS (SELECT * FROM pg_operator WHERE oid = %1)")
										  .arg(oidString()));
}
