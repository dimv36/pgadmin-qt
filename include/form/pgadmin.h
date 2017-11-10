#ifndef PGADMIN_H
#define PGADMIN_H

#include <QMainWindow>
#include <QTreeWidgetItem>

class PGObject;

namespace Ui {
class PgAdmin;
}

class PgAdmin : public QMainWindow
{
	Q_OBJECT

public:
	explicit PgAdmin(QWidget *parent = 0);
	~PgAdmin();

	void closeEvent(QCloseEvent *ev);

private:
	void readSettings();
	void writeSettings();

private slots:
	void slotAddConnection();
	void slotRefreshObject(PGObject *);
	void slotBrowserItemClicked(QTreeWidgetItem *item, int);
	void slotBrowserItemDoubleClicked(QTreeWidgetItem *item, int);

private:
	Ui::PgAdmin *_ui;
	PGObject *_servers;
};

#endif // PGADMIN_H
