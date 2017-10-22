#include "pguser.h"

PGUser::PGUser()
: PGObject(COLLECTION_USERS, QObject::tr("Login roles"), QIcon(":/users.png"))
{

}

void PGUser::refreshProperties(PropertyTable *)
{

}
