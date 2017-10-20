#ifndef PGSET_H
#define PGSET_H

#include <libpq-fe.h>

class PGConnection;

class PGSet
{

public:
	PGSet();
	PGSet(PGresult *result, PGConnection *connection, bool needColumnQuoting);
	~PGSet();

protected:
};

#endif // PGSET_H
