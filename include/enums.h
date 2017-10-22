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
	COLLECTION_USERS,
	COLLECTION_TABLESPACES,
	COLLECTION_GROUPS,

	COLLECTION_LAST_ITEM,

	OBJECT_SERVER,
	OBJECT_DATABASE,
	OBJECT_USER,
	OBJECT_TABLESPACE,
	OBJECT_GROUP,

	OBJECT_LAST_ITEM
} ObjectType;

#endif // ENUMS_H
