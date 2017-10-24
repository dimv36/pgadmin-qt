#include <QMenu>
#include "widget/objectbrowser.h"
#include "schema/pgobject.h"


ObjectBrowser::ObjectBrowser(QWidget *parent)
: QTreeWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this,
			SIGNAL(customContextMenuRequested(const QPoint&)),
			this,
			SLOT(slotCustomContextMenuRequested(const QPoint&)));
}

void ObjectBrowser::addItem(PGObject *item, PGObject *parent)
{
	if (!parent)
		addTopLevelItem(item);
	else
		parent->addChild(item);
}

void ObjectBrowser::showContextMenu(PGObject *object, const QPoint &pos)
{
	QMenu menu;

	object->formContextMenu(&menu);
	QAction *action = menu.exec(pos);
	if (action)
		emit signalRefreshItem(object);
}

void ObjectBrowser::slotCustomContextMenuRequested(const QPoint &point)
{
	QTreeWidgetItem *item = itemAt(point);

	if (item)
	{
		PGObject *object = dynamic_cast<PGObject *>(item);

		showContextMenu(object, viewport()->mapToGlobal(point));
	}
}
