#include "propertytable.h"
#include <QDebug>
#include <QHeaderView>

PropertyTable::PropertyTable(QWidget *parent)
: QTableWidget(parent)
{
	resizeColumnsToContents();
	verticalHeader()->setStretchLastSection(true);
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

void PropertyTable::addRow(const QString &property, const bool &value, const QIcon &icon)
{
	QString valuestr = value ? QObject::tr("yes") : QObject::tr("no");
	addRow(property, valuestr, icon);
	resizeColumnsToContents();
	horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void PropertyTable::removeRows()
{
	setRowCount(0);
}
