#ifndef PROPERTYTABLE_H
#define PROPERTYTABLE_H

#include <QObject>
#include <QTableWidget>
#include "enums.h"

class PropertyTable : public QTableWidget
{
	Q_OBJECT
public:
	PropertyTable(QWidget *parent = 0);

	void setHeaders(const PropertiesType type = PropertiesKeyValue);

	void addRow(const QString &property, const QString &value, const QIcon &icon = QIcon(":/property.png"));
	void addRow(const QString &property, const bool &value, const QIcon &icon = QIcon(":/property.png"));
	void removeRows();

private:
	typedef enum ColumType
	{
		ColumnPropery,
		ColumnValue,
		ColumnCount
	} ColumType;
};

#endif // PROPERTYTABLE_H
