#include <QMenu>
#include "widget/objectbrowser.h"
#include "schema/pgobject.h"


ObjectBrowser::ObjectBrowser(QWidget *parent)
: QTreeWidget(parent),
  _objectMenu(nullptr)
{
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this,
			SIGNAL(customContextMenuRequested(const QPoint &)),
			this,
			SLOT(slotCustomContextMenuRequested(const QPoint &)));

	connect(this,
			SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this,
			SLOT(slotRefreshMenu()));
}

void ObjectBrowser::setEditMenu(const QMenu *menu)
{
	_objectMenu = (QMenu *) menu;
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
	{
		if (action->text() == QObject::tr("Delete"))
			emit signalRefreshItem(nullptr);
		else
			emit signalRefreshItem(object);
	}
}

void ObjectBrowser::slotRefreshMenu()
{
	_objectMenu->clear();

	QTreeWidgetItem *curItem = currentItem();
	if (curItem)
	{
		PGObject *currentObject = dynamic_cast<PGObject *>(curItem);
		currentObject->formContextMenu(_objectMenu);
	}
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
