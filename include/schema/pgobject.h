#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableWidget>
#include <QPair>
#include <QDebug>
#include "enums.h"
#include "db/pgconnection.h"
#include "widget/propertytable.h"


class PGObject : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	PGObject(const PGConnection *connection, ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon(), const QIcon &objIcon = QIcon());

	ObjectType objectType() const;
	QString objectName() const;

	void addChild(PGObject *object, bool unique = true);
	void removeChildrens();

	void setConnection(PGConnection *connection);

	virtual PGObject *appendObject(const PGConnection *, const QString &);
	void refreshProperties(PropertyTable *tab);
	virtual void refreshObjectProperties(PropertyTable *) = 0;

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
	PGConnection *_connection;

	QString _propertiesSQL;
	QIcon _objectIcon;
};

#endif // PGOBJECT_H
