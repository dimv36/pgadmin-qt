#include <QSettings>
#include <QDir>
#include <QFile>
#include "form/pgadmin.h"
#include "ui_pgadmin.h"
#include "dialog/connectiondialog.h"
#include "schema/pgserver.h"
#include "schema/pgdatabase.h"
#include "settings.h"

PgAdmin::PgAdmin(QWidget *parent)
: QMainWindow(parent),
  _ui(new Ui::PgAdmin)
{
	_ui->setupUi(this);
	_servers = new PGServer();
	readSettings();
	_ui->_objectBrowser->addItem(_servers);
	_ui->_objectBrowser->expandToDepth(0);

	connect(_ui->_objectBrowser,
			SIGNAL(itemClicked(QTreeWidgetItem *, int)),
			this,
			SLOT(slotBrowserItemClicked(QTreeWidgetItem *, int)));
	connect(_ui->_objectBrowser,
			SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			this,
			SLOT(slotBrowserItemDoubleClicked(QTreeWidgetItem *, int)));
	connect(_ui->_objectBrowser,
			SIGNAL(signalRefreshItem(PGObject *)),
			this,
			SLOT(slotRefreshObject(PGObject *)));
}

PgAdmin::~PgAdmin()
{
	writeSettings();
	delete _ui;
}

void PgAdmin::readSettings()
{
	QString filepath;

#if defined(Q_OS_LINUX)
	filepath = QDir::homePath() + "/.config/pgadmin-qt.conf";
#elif defined(Q_OS_WIN32)

#elif defined(Q_OS_MAC)

#endif

	if (!QFileInfo(filepath).exists())
		return;

	QSettings settings(filepath, QSettings::IniFormat);

	settings.beginGroup(SETTINGS_MAIN);
	int x = settings.value(SETTINGS_MAIN_X).toInt();
	int y = settings.value(SETTINGS_MAIN_Y).toInt();
	QSize size = settings.value(SETTINGS_MAIN_SIZE).toSize();
	QByteArray vstate = settings.value(SETTINGS_MAIN_VSLIDER_POS).toByteArray();
	QByteArray hstate = settings.value(SETTINGS_MAIN_HSLIDER_POS).toByteArray();
	settings.endGroup();

	// Apply widget settings
	setGeometry(x, y, size.width(), size.height());
	_ui->_splitterVertical->restoreState(vstate);
	_ui->_splitterHorizontal->restoreState(hstate);

	// Read connections information
	int concount = settings.beginReadArray(SETTINGS_CONNECTIONS);

	for (int i = 0; i < concount; ++i)
	{
		settings.setArrayIndex(i);

		QString conname = settings.value(SETTINGS_CONNECTION_NAME).toString();
		QString host = settings.value(SETTINGS_CONNECTION_HOST).toString();
		int port = settings.value(SETTINGS_CONNECTION_PORT).toInt();
		QString dbname = settings.value(SETTINGS_CONNECTION_DBNAME).toString();
		QString username = settings.value(SETTINGS_CONNECTION_USERNAME).toString();
		QString password = settings.value(SETTINGS_CONNECTION_PASSWORD).toString();

		// Add new server item
		PGServer *server = new PGServer(conname, host, port,
										dbname, username, password);
		_ui->_objectBrowser->addItem(server, _servers);
	}
	settings.endArray();
}

void PgAdmin::writeSettings()
{
	QString filepath;

#if defined(Q_OS_LINUX)
	filepath = QDir::homePath() + "/.config/pgadmin-qt.conf";
#elif defined(Q_OS_WIN32)

#elif defined(Q_OS_MAC)

#endif
	QSettings settings(filepath, QSettings::IniFormat);

	settings.beginGroup(SETTINGS_MAIN);
	settings.setValue(SETTINGS_MAIN_X, x());
	settings.setValue(SETTINGS_MAIN_Y, y());
	settings.setValue(SETTINGS_MAIN_SIZE, size());
	settings.setValue(SETTINGS_MAIN_VSLIDER_POS, _ui->_splitterVertical->saveState());
	settings.setValue(SETTINGS_MAIN_HSLIDER_POS, _ui->_splitterHorizontal->saveState());
	settings.endGroup();

	if (_servers->childCount())
	{
		settings.beginWriteArray(SETTINGS_CONNECTIONS, _servers->childCount());
		for (int i = 0; i < _servers->childCount(); i++)
		{
			PGServer *server = dynamic_cast<PGServer *>(_servers->child(i));

			settings.setArrayIndex(i);

			settings.setValue(SETTINGS_CONNECTION_NAME, server->connectionName());
			settings.setValue(SETTINGS_CONNECTION_HOST, server->host());
			settings.setValue(SETTINGS_CONNECTION_PORT, server->port());
			settings.setValue(SETTINGS_CONNECTION_DBNAME, server->dbname());
			settings.setValue(SETTINGS_CONNECTION_USERNAME, server->username());
			settings.setValue(SETTINGS_CONNECTION_PASSWORD, server->password());
		}
		settings.endArray();
	}
}

void PgAdmin::slotRefreshObject(PGObject *object)
{
	_ui->_quickObjectView->clear();
	_ui->_propertiesWidget->removeRows();

	if (object)
	{
		object->refreshProperties(_ui->_propertiesWidget);
		object->refresh(_ui->_objectProperties);
	}
}

void PgAdmin::on__actionAddConnection_triggered()
{
	ConnectionDialog dialog;

	if (dialog.exec() == QDialog::Accepted)
	{
		QString connection = dialog.connectionName();
		QString host = dialog.host();
		int port = dialog.port();
		QString dbname = dialog.dbnane();
		QString username = dialog.username();
		QString password = dialog.password();

		// Add new server item
		PGServer *server = new PGServer(connection, host, port, dbname, username, password);
		_ui->_objectBrowser->addItem(server, _servers);
		_ui->_objectBrowser->expandAll();
	}
}

void PgAdmin::slotBrowserItemClicked(QTreeWidgetItem *item, int)
{
	PGObject *object = dynamic_cast<PGObject *>(item);
	slotRefreshObject(object);
}

void PgAdmin::slotBrowserItemDoubleClicked(QTreeWidgetItem *item, int)
{
	PGObject *object = dynamic_cast<PGObject *>(item);

	switch (object->objectType())
	{
		case OBJECT_SERVER:
		{
			PGServer *server = dynamic_cast<PGServer *>(item);
			server->connect();
		}
		break;

		case OBJECT_DATABASE:
		{
			PGDatabase *database = dynamic_cast<PGDatabase *>(item);
			database->connect();
		}
		break;

		default:
			break;
	}
}
