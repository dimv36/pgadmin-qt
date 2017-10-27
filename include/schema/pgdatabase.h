#ifndef PGDATABASE_H
#define PGDATABASE_H

#include "pgobject.h"

class PGDatabase : public PGObject
{
	Q_OBJECT
public:
	PGDatabase(const PGConnection *connection, const QString &name);
	PGDatabase(PGConnection *connection);

	void connect();
	bool connected() const;
	void disconnect();

	virtual PGDatabase *appendObject(const PGConnection *connection, const QString &);
	virtual void refreshObjectProperties(PropertyTable *);

protected slots:
	void slotReconnect();
	void slotConnect();
	void slotDisconnect();

protected:
	virtual void formContextMenu(QMenu *menu);
};

#endif // PGDATABASE_H
