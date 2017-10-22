#include "pgtablespace.h"

PGTablespace::PGTablespace()
: PGObject(COLLECTION_TABLESPACES, QObject::tr("Tablespaces"), QIcon(":/tablespaces.png"))
{

}

PGTablespace::~PGTablespace()
{

}

void PGTablespace::refreshProperties(PropertyTable *)
{

}
