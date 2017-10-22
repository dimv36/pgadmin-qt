#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionDialog(QWidget *parent = 0);
	~ConnectionDialog();

	QString connectionName() const;
	QString host() const;
	int port() const;
	QString dbnane() const;
	QString username() const;
	QString password() const;

public slots:
	void checkEnable();

private:
	Ui::ConnectionDialog *_ui;
};

#endif // CONNECTIONDIALOG_H
