#ifndef PROPERTYTABLE_H
#define PROPERTYTABLE_H

#include <QObject>
#include <QTableWidget>

class PropertyTable : public QTableWidget
{
	Q_OBJECT
public:
	PropertyTable(QWidget *parent = 0);

	void addRow(const QString &property, const QString &value, const QIcon &icon = QIcon());
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
