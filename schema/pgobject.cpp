#include "schema/pgobject.h"
#include <QObject>
#include <QMenu>

PGObject::PGObject(const PGConnection *connection, ObjectType objtype, const QString &name, const QIcon &icon, const QIcon &objIcon)
: _objtype(objtype),
  _name(name),
  _objectIcon(objIcon)
{
	setText(ColumnText, name);
	setIcon(ColumnText, icon);
	_connection = (PGConnection *) connection;
}

ObjectType PGObject::objectType() const
{
	return _objtype;
}

QString PGObject::objectName() const
{
	return _name;
}

PGObject *PGObject::parentItem() const
{
	QTreeWidgetItem *parent = QTreeWidgetItem::parent();
	return dynamic_cast<PGObject *>(parent);
}

void PGObject::addChild(PGObject *object, bool unique)
{
	bool found = false;

	if (!object)
		return;

	for (int i = 0; i < childCount(); i++)
	{
		PGObject *ch = dynamic_cast<PGObject *> (child(i));

		if ((ch->objectName() == object->objectName()) &&
			(ch->objectType() == object->objectType()))
		{
			found = true;
			break;
		}
	}
	if (unique && found)
		return;
	QTreeWidgetItem::addChild(object);
}

void PGObject::removeChildrens()
{
	while (true)
	{
		if (!childCount())
			break;

		PGObject *item = dynamic_cast<PGObject *>(child(0));
		delete item;
	}
}

void PGObject::setConnection(PGConnection *connection)
{
	_connection = connection;
}

PGObject *PGObject::appendObject(const PGConnection *, const QString &)
{
	return nullptr;
}

void PGObject::refreshProperties(PropertyTable *tab)
{
	if (_objtype < COLLECTION_LAST_ITEM)
	{
		if (!_propertiesSQL.isEmpty())
		{
			PGSet *set = _connection->executeSet(_propertiesSQL);
			if (set)
			{
				tab->setHeaders(PropertiesSummary);
				while (!set->eof())
				{
					Oid objectId = set->oidValue("oid");
					QString objname = set->value("objname");
					QString owner = set->hasColumn("owner") ? set->value("owner") : QString();
					QString comment = set->value("comment");

					qDebug() << "objname: " << objname << " oid: " << objectId << endl;

					// Add new object as child
					PGObject *object = appendObject(_connection, objname);
					object->setOid(objectId);
					addChild(object);

					tab->addRowSummary(objname, owner, comment, _objectIcon);
					set->moveNext();
				}
				// Update objects counter
				setText(ColumnText, QString("%1 (%2)").arg(objectName()).arg(set->rowsCount()));
			}
		}
	}
	else
		refreshObjectProperties(tab);
}

void PGObject::formContextMenu(QMenu *menu)
{
	if (_objtype != COLLECTION_SERVERS)
	{
		menu->addAction(QObject::tr("Refresh"), this, SLOT(slotActionRefresh()));
		menu->addSeparator();
	}
}

bool PGObject::isSystemObject()
{
	return (_oid <  FirstNormalObjectId);
}

void PGObject::setOid(const Oid oid)
{
	_oid = oid;
}

Oid PGObject::oid() const
{
	return _oid;
}

void PGObject::slotActionRefresh()
{
	emit signalDataChanged(this);
}
