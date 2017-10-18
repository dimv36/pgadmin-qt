#include <QSettings>
#include <QDir>
#include <QFile>
#include "pgadmin.h"
#include "ui_pgadmin.h"
#include "connectiondialog.h"
#include "pgserver.h"
#include "settings.h"

PgAdmin::PgAdmin(QWidget *parent)
: QMainWindow(parent),
  _ui(new Ui::PgAdmin)
{
	_ui->setupUi(this);
	_servers = new PGServer();
	readSettings();
	_ui->_objectBrowser->addTopLevelItem(_servers);
	_ui->_objectBrowser->expandAll();
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

		PGServerSettings serverSetting;
		QString conname = settings.value(SETTINGS_CONNECTION_NAME).toString();
		QString host = settings.value(SETTINGS_CONNECTION_HOST).toString();
		int port = settings.value(SETTINGS_CONNECTION_PORT).toInt();
		QString dbname = settings.value(SETTINGS_CONNECTION_DBNAME).toString();
		QString username = settings.value(SETTINGS_CONNECTION_USERNAME).toString();
		QString password = settings.value(SETTINGS_CONNECTION_PASSWORD).toString();

		serverSetting[SETTINGS_CONNECTION_NAME] = conname;
		serverSetting[SETTINGS_CONNECTION_HOST] = host;
		serverSetting[SETTINGS_CONNECTION_PORT] = port;
		serverSetting[SETTINGS_CONNECTION_DBNAME] = dbname;
		serverSetting[SETTINGS_CONNECTION_USERNAME] = username;
		serverSetting[SETTINGS_CONNECTION_PASSWORD] = password;

		// Add new server item
		PGServer *server = new PGServer(conname, host, port,
										dbname, username, password);
		_servers->addChild(server);

		_serverSettings.push_back(serverSetting);
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
	if (_serverSettings.count())
	{
		int i = 0;

		settings.beginWriteArray(SETTINGS_CONNECTIONS, _serverSettings.count());
		for (auto it = _serverSettings.begin();
			 it != _serverSettings.end();
			 ++it)
		{
			settings.setArrayIndex(i);
			PGServerSettings serverSetting = *it;
			for (auto sit : serverSetting.keys())
				settings.setValue(sit, serverSetting.value(sit));
			++i;
		}
		settings.endArray();
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

		// Save connection settings
		PGServerSettings serverSetting;
		serverSetting[SETTINGS_CONNECTION_NAME] = connection;
		serverSetting[SETTINGS_CONNECTION_HOST] = host;
		serverSetting[SETTINGS_CONNECTION_PORT] = port;
		serverSetting[SETTINGS_CONNECTION_DBNAME] = dbname;
		serverSetting[SETTINGS_CONNECTION_USERNAME] = username;
		serverSetting[SETTINGS_CONNECTION_PASSWORD] = password;
		_serverSettings.push_back(serverSetting);

		// Add new server item
		PGServer *server = new PGServer(connection, host, port, dbname, username, password);
		_servers->addChild(server);
		_ui->_objectBrowser->expandAll();
	}
}

void PgAdmin::on__objectBrowser_itemClicked(QTreeWidgetItem *item, int column)
{
	qDebug() << "clicked" << endl;

	PGObject *object = dynamic_cast<PGObject *> (item);

	switch (object->objectType())
	{
		case OBJECT_SERVER:
			(dynamic_cast<PGServer *>(item))->connect();
			break;

		default:
			break;
	}
}
