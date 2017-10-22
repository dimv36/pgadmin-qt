#include "pggroup.h"

PGGroup::PGGroup()
: PGObject(COLLECTION_GROUPS, QObject::tr("Groups"), QIcon(":/groups.png"))
{

}

void PGGroup::refreshProperties(PropertyTable *)
{

}
