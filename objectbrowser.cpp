#include <QMenu>
#include "objectbrowser.h"
#include "pgobject.h"


ObjectBrowser::ObjectBrowser(QWidget *parent)
: QTreeWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this,
			SIGNAL(customContextMenuRequested(const QPoint&)),
			this,
			SLOT(onCustomContextMenuRequested(const QPoint&)));
}

void ObjectBrowser::addTopItem(PGObject *obj)
{
	addTopLevelItem(obj);
}

void ObjectBrowser::showContextMenu(PGObject *object, const QPoint &pos)
{
	QMenu menu;

	object->formContextMenu(&menu);
	menu.exec(pos);
}

void ObjectBrowser::onCustomContextMenuRequested(const QPoint &point)
{
	QTreeWidgetItem* item = itemAt(point);

	if (item)
	{
		PGObject *object = dynamic_cast<PGObject *>(item);

		showContextMenu(object, viewport()->mapToGlobal(point));
	}
}
