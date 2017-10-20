#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableWidget>
#include <QPair>
#include "enums.h"

class ObjectBrowser;
class PropertyTable;


class PGObject : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	PGObject(ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon());
	PGObject(ObjectType objtype, bool isCollection);
	ObjectType objectType() const;

public:
	virtual void setMainObjectProperties(PropertyTable *tab) = 0;

protected:
	friend class ObjectBrowser;

	virtual void formContextMenu(QMenu *menu);
	virtual void setObjectProperties(QTabWidget *tab) {}
	virtual void refreshItem() {}

protected slots:
	void slotActionRefresh();

protected:
	ObjectType _objtype;
	QString _name;
};

#endif // PGOBJECT_H
