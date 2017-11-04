#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <QTreeWidgetItem>
#include <QMenu>
#include <QTableWidget>
#include <QPair>
#include <QDebug>
#include "enums.h"
#include "db/pgconnection.h"
#include "db/pgproperties.h"
#include "widget/propertytable.h"
#include "widget/objectbrowser.h"

/* as defined at access/transam.h */
#define FirstNormalObjectId 16384

class PGObject : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	PGObject(const PGConnection *connection, ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon(), const QIcon &objIcon = QIcon());

	ObjectType objectType() const;
	QString objectName() const;

	PGObject *parentItem() const;

	void addChild(PGObject *object, bool unique = true);
	void removeChildrens();

	void setConnection(PGConnection *connection);

	void refreshProperties(PropertyTable *tab);
	virtual void refreshObjectProperties(PropertyTable *) = 0;

	virtual void refresh(QTabWidget *) {}
	virtual void formContextMenu(QMenu *menu);
	virtual void appendCollectionItems() {}

	virtual bool isSystemObject();

	void setObjectAttribute(const QString &attribute, const QVariant &value);

	int intObjectAttribute(const QString &attribute);
	Oid oidObjectAttribute(const QString &attribute);
	bool boolObjectAttribute(const QString &attribute);
	QString stringObjectAttribute(const QString &attribute);

protected:
	ObjectBrowser *browser() const;

signals:
	void signalDataChanged(PGObject *);

protected slots:
	void slotActionRefresh();

protected:
	ObjectType _objtype;
	PGConnection *_connection;

	PGProperties _objectProperties;

	QIcon _objectIcon;
};

#endif // PGOBJECT_H
