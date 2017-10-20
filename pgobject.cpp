#include "pgobject.h"
#include "objectbrowser.h"
#include "propertytable.h"
#include <QDebug>

PGObject::PGObject(ObjectType objtype, const QString &name, const QIcon &icon)
: _objtype(objtype),
  _name(name)
{
	setText(ColumnText, name);
	setIcon(ColumnText, icon);
}

ObjectType PGObject::objectType() const
{
	return _objtype;
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
	return refreshItem();
}

