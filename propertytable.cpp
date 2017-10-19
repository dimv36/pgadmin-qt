#include "propertytable.h"
#include <QDebug>

PropertyTable::PropertyTable(QWidget *parent)
: QTableWidget(parent)
{
}

void PropertyTable::addRow(const QString &property, const QString &value, const QIcon &icon)
{
	int rows = rowCount();
	setRowCount(rows + 1);

	QTableWidgetItem *propertyItem = new QTableWidgetItem(property);
	propertyItem->setIcon(icon);

	setItem(rows, ColumnPropery, propertyItem);
	setItem(rows, ColumnValue, new QTableWidgetItem(value));
}

void PropertyTable::removeRows()
{
	setRowCount(0);
}
