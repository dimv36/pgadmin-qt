#ifndef INCLUDES_H
#define INCLUDES_H

typedef unsigned int Oid;

#ifndef InvalidOid
	#define InvalidOid ((Oid) 0)
#endif

#ifndef atooid
	#define atooid(_data) (strtoul((_data), NULL, 10))
#endif

#endif // INCLUDES_H
