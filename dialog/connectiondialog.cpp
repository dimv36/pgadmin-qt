#include "dialog/connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QDebug>


ConnectionDialog::ConnectionDialog(QWidget *parent)
: QDialog(parent),
  _ui(new Ui::ConnectionDialog)
{
	_ui->setupUi(this);
	_ui->_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
	QIntValidator *intValidator = new QIntValidator();
	intValidator->setRange(1000, 100000);
	_ui->_portLineEdit->setValidator(intValidator);
	_ui->_nameLineEdit->setPlaceholderText(QObject::tr("Please, set name for connection"));
	_ui->_passwordLineEdit->setEchoMode(QLineEdit::Password);
}

ConnectionDialog::~ConnectionDialog()
{
	delete _ui;
}

QString ConnectionDialog::connectionName() const
{
	return _ui->_nameLineEdit->text();
}

QString ConnectionDialog::host() const
{
	return _ui->_hostLineEdit->text();
}

int ConnectionDialog::port() const
{
	return _ui->_portLineEdit->text().toInt();
}

QString ConnectionDialog::dbnane() const
{
	return _ui->_databaseLineEdit->text();
}

QString ConnectionDialog::username() const
{
	return _ui->_usernameLineEdit->text();
}

QString ConnectionDialog::password() const
{
	return _ui->_passwordLineEdit->text();
}

void ConnectionDialog::checkEnable()
{
	bool enabled = false;

	if (!_ui->_nameLineEdit->text().isEmpty())
		enabled = true;

	_ui->_buttons->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}
