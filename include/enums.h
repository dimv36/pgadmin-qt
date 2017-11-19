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
	COLLECTION_ACCESSMETHODS,
	COLLECTION_CASTS,
	COLLECTION_EVENTTRIGGERS,
	COLLECTION_FDWS,
	COLLECTION_FOREIGNSERVERS,
	COLLECTION_USERMAPPINGS,
	COLLECTION_EXTENSIONS,
	COLLECTION_LANGUAGES,
	COLLECTION_SCHEMAS,
	COLLECTION_COLLATIONS,
	COLLECTION_CONVERSIONS,
	COLLECTION_TEXTSEARCHCONFIGURATIONS,
	COLLECTION_TEXTSEARCHDICTIONARIES,
	COLLECTION_TEXTSEARCHPARSERS,
	COLLECTION_TEXTSEARCHTEMPLATES,
	COLLECTION_OPERATORS,

	COLLECTION_LAST_ITEM,

	OBJECT_SERVER,
	OBJECT_DATABASE,
	OBJECT_TABLESPACE,
	OBJECT_ROLE,
	OBJECT_USER,
	OBJECT_GROUP,
	OBJECT_ACCESSMETHOD,
	OBJECT_CAST,
	OBJECT_EVENTTRIGGER,
	OBJECT_FDW,
	OBJECT_FOREIGNSERVER,
	OBJECT_USERMAPPING,
	OBJECT_EXTENSION,
	OBJECT_LANGUAGE,
	OBJECT_SCHEMA,
	OBJECT_COLLATION,
	OBJECT_CONVERSION,
	OBJECT_TEXTSEARCHCONFIGURATION,
	OBJECT_TEXTSEARCHDICTIONARY,
	OBJECT_TEXTSEARCHPARSER,
	OBJECT_TEXTSEARCHTEMPLATE,
	OBJECT_OPERATOR,

	OBJECT_LAST_ITEM
} ObjectType;

#endif // ENUMS_H
