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

private:
	void readSettings();
	void writeSettings();

private slots:
	void on__actionAddConnection_triggered();
	void on__objectBrowser_itemClicked(QTreeWidgetItem *item, int);
	void on__objectBrowser_itemDoubleClicked(QTreeWidgetItem *item, int);

private:
	Ui::PgAdmin *_ui;
	PGObject *_servers;

	typedef QMap<QString, QVariant> PGServerSettings;
	QVector <PGServerSettings> _serverSettings;
};

#endif // PGADMIN_H
