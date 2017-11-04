#ifndef PROPERTYTABLE_H
#define PROPERTYTABLE_H

#include <QObject>
#include <QTableWidget>
#include "enums.h"
#include "includes.h"

class PropertyTable : public QTableWidget
{
	Q_OBJECT
public:
	PropertyTable(QWidget *parent = 0);

	void setHeaders(const PropertiesType type = PropertiesKeyValue);

	void addRow(const QString &property, const QString &value, const QIcon &icon = QIcon(":/property.png"));
	void addRow(const QString &property, const bool &value, const QIcon &icon = QIcon(":/property.png"));
	void addRow(const QString &property, const Oid &value, const QIcon &icon = QIcon(":/property.png"));
	void addRow(const QString &property, const int &value, const QIcon &icon = QIcon(":/property.png"));

	void addRowSummary(const QString &objname, const QString &owner, const QString &comment, const QIcon &icon = QIcon());
	void removeRows();

private:
	typedef enum ColumnTypeProperty
	{
		ColumnPropery,
		ColumnValue,
		ColumnTypePropertyLast
	} ColumnType;

	typedef enum ColumnTypeObjectSummary
	{
		ColumnName,
		ColumnOwner,
		ColumnComment,
		ColumnTypeObjectSummaryLast
	} ColumnTypeObjectSummary;

	PropertiesType _propertiesType;
};

#endif // PROPERTYTABLE_H
