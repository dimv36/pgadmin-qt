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
	PGObject(const PGConnection *connection, ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon());

	virtual void afterConstruction();

	ObjectType objectType() const;
	QString objectName() const;
	QString oidString() const;

	bool isCollectionItem() const;

	PGObject *parentItem() const;

	void addChild(PGObject *object, bool unique = true);
	void removeChildrens();

	void setConnection(PGConnection *connection);

	void showObjectProperties(PropertyTable *);
	virtual void showSingleObjectProperties(PropertyTable *) = 0;

	virtual void appendOrRefreshObject(PGObject * = nullptr) {}
	virtual void formContextMenu(QMenu *menu);
	virtual void appendCollectionItems() {}
	virtual bool exists() const = 0;

	virtual bool isSystemObject();

	void setObjectAttribute(const QString &attribute, const QVariant &value);

	int intObjectAttribute(const QString &attribute);
	Oid oidObjectAttribute(const QString &attribute);
	bool boolObjectAttribute(const QString &attribute);
	QString stringObjectAttribute(const QString &attribute);

	void parseSecurityLabels(const QString &providers, const QString &labels);
	void appendSecurityLabels(PropertyTable *tab);

public slots:
	void refresh();

protected:
	ObjectBrowser *browser() const;
	void refreshCollectionTitle();

	void setObjectAttribute(const QString &attribute, const QVector<QVariant> &value);

protected:
	ObjectType _objtype;
	PGConnection *_connection;

	PGProperties _objectProperties;
};

template <class T>
T *newPGObject(const PGConnection *connection)
{
	T* pgobject = new T(connection);
	pgobject->afterConstruction();
	return pgobject;
}

#endif // PGOBJECT_H
