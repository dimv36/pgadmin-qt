#ifndef PGDATABASE_H
#define PGDATABASE_H

#include "pgobject.h"

class PGDatabase : public PGObject
{
	Q_OBJECT
public:
	PGDatabase(const PGConnection *connection, const QString &name);
	PGDatabase(const PGConnection *connection);

	void connect();
	bool connected() const;
	void disconnect();

	virtual void appendOrRefreshObject(PGObject * = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
	virtual bool exists() const;

protected slots:
	void slotReconnect();
	void slotConnect();
	void slotDisconnect();

protected:
	virtual void formContextMenu(QMenu *menu);
	void getDatabaseSettings();

private:
	void setDefaultParams();

private:
	QString _searchPath;

	QString _defPrivsOnTables;
	QString _defPrivsOnSeqs;
	QString _defPrivsOnFuncs;
	QString _defPrivsOnTypes;
};

#endif // PGDATABASE_H
