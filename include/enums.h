#ifndef ENUMS_H
#define ENUMS_H

typedef enum Columns
{
	ColumnText,
	ColumnCount
} Columns;

typedef enum PropertiesType
{
	PropertiesKeyValue,
	PropertiesSummary
} PropertiesType;

typedef enum ObjectType
{
	COLLECTION_SERVERS,
	COLLECTION_DATABASES,
	COLLECTION_TABLESPACES,
	COLLECTION_ROLES,
	COLLECTION_USERS,
	COLLECTION_GROUPS,

	COLLECTION_LAST_ITEM,

	OBJECT_SERVER,
	OBJECT_DATABASE,
	OBJECT_TABLESPACE,
	OBJECT_ROLE,
	OBJECT_USER,
	OBJECT_GROUP,

	OBJECT_LAST_ITEM
} ObjectType;

#define IsCollectionItem(_obj) ((_obj) < COLLECTION_LAST_ITEM)

#endif // ENUMS_H
