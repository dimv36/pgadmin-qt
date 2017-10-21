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
	PGObject(ObjectType objtype, bool isCollection);
	ObjectType objectType() const;
	QString objectName() const;

public:
	virtual void refreshProperties(PropertyTable *) = 0;
	virtual void refresh(QTabWidget *) {}
	virtual void formContextMenu(QMenu *menu);

signals:
	void signalDataChanged(PGObject *);

protected slots:
	void slotActionRefresh();

protected:
	ObjectType _objtype;
	QString _name;
};

#endif // PGOBJECT_H
