#include "schema/pgobject.h"
#include "db/pgproperties.h"
#include <QObject>
#include <QMenu>

PGObject::PGObject(const PGConnection *connection, ObjectType objtype, const QString &name, const QIcon &icon)
: _objtype(objtype)
{
	_objectProperties.setName(name);
	_connection = (PGConnection *) connection;
	setText(ColumnText, name);
	setIcon(ColumnText, icon);
}

void PGObject::afterConstruction()
{
	if (isCollectionItem())
	{
		appendOrRefreshObject();
		refreshCollectionTitle();
	}
}

ObjectType PGObject::objectType() const
{
	return _objtype;
}

QString PGObject::objectName() const
{
	return _objectProperties.name();
}

QString PGObject::oidString() const
{
	return _objectProperties.oidString();
}

bool PGObject::isCollectionItem() const
{
	return (_objtype < COLLECTION_LAST_ITEM);
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

		if (object->isCollectionItem() &&
			(ch->objectType() == object->objectType()))
		{
			found = true;
			break;
		}

		if ((ch->_objectProperties.name() == _objectProperties.name()) &&
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

void PGObject::showObjectProperties(PropertyTable *tab)
{
	if (_objtype < COLLECTION_LAST_ITEM)
	{
		tab->setHeaders(PropertiesSummary);
		for (int i = 0; i < childCount(); i++)
		{
			PGObject *childObject = dynamic_cast<PGObject *> (child(i));

			tab->addRowSummary(childObject->_objectProperties.name(),
							   childObject->_objectProperties.owner(),
							   childObject->_objectProperties.comment(),
							   childObject->icon(ColumnText));
		}
	}
	else
	{
		tab->setHeaders(PropertiesKeyValue);
		showSingleObjectProperties(tab);
	}
}

void PGObject::refresh()
{
	if (!isCollectionItem())
		appendOrRefreshObject(this);
	else
	{
		removeChildrens();
		appendOrRefreshObject();
		refreshCollectionTitle();
	}
}

void PGObject::formContextMenu(QMenu *menu)
{
	if (_objtype != COLLECTION_SERVERS)
	{
		menu->addAction(QObject::tr("Refresh"), this, SLOT(refresh()));
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

void PGObject::refreshCollectionTitle()
{
	if (!isCollectionItem())
		return;

	setText(ColumnText, QString("%1 (%2)").arg(objectName()).arg(childCount()));
}

void PGObject::parseSecurityLabels(const QString &providers, const QString &labels)
{
	PGKeyValueSettings seclabelsArray;

	QString providersStr = providers;
	QString labelsStr = labels;

	// Because we have an arrays, remove symbols { and }
	providersStr = providersStr.remove('{').remove('}');
	labelsStr = labelsStr.remove('{').remove('}');

	// Next, split them by ',' symbol
	QStringList providersList = providersStr.split(',');
	QStringList labelsList = labelsStr.split(',');

	for (int i = 0; i < providersList.size(); i++)
	{
		if (i >= labelsList.size())
			break;

		PGKeyValueSetting seclabel(providersList.at(i), labelsList.at(i));
		seclabelsArray.push_back(seclabel);
	}
	setObjectAttribute("seclabels", QVariant::fromValue(seclabelsArray));
}

void PGObject::appendSecurityLabels(PropertyTable *tab)
{
	PGKeyValueSettings seclabels = _objectProperties.seclabels();

	for (auto it = seclabels.begin(); it != seclabels.end(); ++it)
	{
		PGKeyValueSetting seclabel = *it;

		if (seclabel.first.isEmpty() && seclabel.second.isEmpty())
			return;

		tab->addRow(QObject::tr("Security label (%1)").arg(seclabel.first),
					seclabel.second);
	}
}
