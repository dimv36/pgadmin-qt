#include "schema/pgobject.h"
#include <QObject>
#include <QMenu>

PGObject::PGObject(const PGConnection *connection, ObjectType objtype, const QString &name, const QIcon &icon, const QIcon &objIcon)
: _objtype(objtype),
  _objectIcon(objIcon)
{
	_objectProperties.setName(name);
	_connection = (PGConnection *) connection;
	setText(ColumnText, name);
	setIcon(ColumnText, icon);
}

ObjectType PGObject::objectType() const
{
	return _objtype;
}

QString PGObject::objectName() const
{
	return _objectProperties.name();
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

void PGObject::refreshProperties(PropertyTable *tab)
{
	if (_objtype < COLLECTION_LAST_ITEM)
	{
		tab->setHeaders(PropertiesSummary);
		for (int i = 0; i < childCount(); i++)
		{
			PGObject *childObject = dynamic_cast<PGObject *> (child(i));

			tab->addRowSummary(childObject->_objectProperties.name(),
							   childObject->_objectProperties.owner(),
							   childObject->_objectProperties.comment());
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
	return (_objectProperties.oid() <  FirstNormalObjectId);
}

void PGObject::setObjectAttribute(const QString &attribute, const QVariant &value)
{
	_objectProperties[attribute] = value;
}

int PGObject::intObjectAttribute(const QString &attribute)
{
	return _objectProperties.intValue(attribute);
}

Oid PGObject::oidObjectAttribute(const QString &attribute)
{
	return _objectProperties.oidValue(attribute);
}

bool PGObject::boolObjectAttribute(const QString &attribute)
{
	return _objectProperties.boolValue(attribute);
}

QString PGObject::stringObjectAttribute(const QString &attribute)
{
	return _objectProperties.stringValue(attribute);
}

ObjectBrowser *PGObject::browser() const
{
	return dynamic_cast<ObjectBrowser *>(treeWidget());
}

void PGObject::slotActionRefresh()
{
	emit signalDataChanged(this);
}
