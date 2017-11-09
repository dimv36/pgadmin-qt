#ifndef PGEVENTTRIGGER_H
#define PGEVENTTRIGGER_H

#include "schema/pgobject.h"

class PGEventTrigger : public PGObject
{
	Q_OBJECT
public:
	PGEventTrigger(const PGConnection *connection, const QString &name);
	PGEventTrigger(const PGConnection *connection);

	virtual bool exists() const;
	virtual void appendOrRefreshObject(PGObject *object = nullptr);
	virtual void showSingleObjectProperties(PropertyTable *);
};

#endif // PGEVENTTRIGGER_H
