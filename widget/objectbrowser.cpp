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
			SLOT(onCustomContextMenuRequested(const QPoint&)));
}

void ObjectBrowser::addItem(PGObject *item, PGObject *parent)
{
	if (!parent)
		addTopLevelItem(item);
	else
		parent->addChild(item);
	connect(item, SIGNAL(signalDataChanged(PGObject *)), this, SLOT(slotItemNeedRefreshing(PGObject *)));
}

void ObjectBrowser::showContextMenu(PGObject *object, const QPoint &pos)
{
	QMenu menu;

	object->formContextMenu(&menu);
	QAction *action = menu.exec(pos);
	qDebug() << action << endl;
}

void ObjectBrowser::onCustomContextMenuRequested(const QPoint &point)
{
	QTreeWidgetItem *item = itemAt(point);

	if (item)
	{
		PGObject *object = dynamic_cast<PGObject *>(item);

		showContextMenu(object, viewport()->mapToGlobal(point));
	}
}

void ObjectBrowser::slotItemNeedRefreshing(PGObject *object)
{
	emit signalRefreshItem(object);
}
