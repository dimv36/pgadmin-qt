#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableWidget>
#include <QPair>
#include <QDebug>
#include "enums.h"

class ObjectBrowser;
class PropertyTable;


class PGObject : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	PGObject(ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon());
	ObjectType objectType() const;
	QString objectName() const;

	void addChild(PGObject *object, bool unique = true);
	void removeChildrens();

	virtual void refreshProperties(PropertyTable *) = 0;
	virtual void refresh(QTabWidget *) {}
	virtual void formContextMenu(QMenu *menu);
	virtual void appendCollectionItems() {}

signals:
	void signalDataChanged(PGObject *);

protected slots:
	void slotActionRefresh();

protected:
	ObjectType _objtype;
	QString _name;
};

#endif // PGOBJECT_H
