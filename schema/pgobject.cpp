#include "schema/pgobject.h"

PGObject::PGObject(ObjectType objtype, const QString &name, const QIcon &icon, const QIcon &objIcon)
: _objtype(objtype),
  _name(name),
  _objectIcon(objIcon)
{
	setText(ColumnText, name);
	setIcon(ColumnText, icon);
}

ObjectType PGObject::objectType() const
{
	return _objtype;
}

QString PGObject::objectName() const
{
	return _name;
}

void PGObject::addChild(PGObject *object, bool unique)
{
	bool found = false;

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
					QString datname = set->value("objname");
					QString comment = set->value("comment");

					tab->addRow(datname, comment, _objectIcon);
					set->moveNext();
				}
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

void PGObject::slotActionRefresh()
{
	emit signalDataChanged(this);
}
