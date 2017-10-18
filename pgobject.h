#ifndef PGOBJECT_H
#define PGOBJECT_H

#include <QTreeWidgetItem>
#include <QDebug>
#include <QMenu>
#include "enums.h"

class ObjectBrowser;

class PGObject : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	PGObject(ObjectType objtype, const QString &name = QString(), const QIcon &icon = QIcon(), bool isCollection = false);
	PGObject(ObjectType objtype, bool isCollection);
	ObjectType objectType() const;
//	PGObject(pgaFactory &factory, const QString &newName = QString());

public:
	void addItem(PGObject *obj);

protected:
	friend class ObjectBrowser;

	virtual void formContextMenu(QMenu *menu);
	virtual void refreshItem() {}

protected slots:
	void slotActionRefresh();

protected:
	ObjectType _objtype;
	QString _name;
	bool _isCollection;
	/*
	*  Except column level privileges, column will be always an empty
	*  string in any case
	*/
//	static QString GetPrivileges(const QString &allPattern,
//								  const QString &acl, const QString &grantObject,
//								  const QString &user, const QString &column = "");

//	static QString GetDefaultPrivileges(const QString &strType, const QString &strSupportedPrivs,
//										 const QString &strSchema, const QString &strOrigDefPrivs,
//										 const QString &strNewDefPrivs, const QString &strRole);
////	static QString GetPrivilegeName(wxChar privilege);
//	static bool     findUserPrivs(QString &, QString &, QString &);

//	static int GetTypeId(const QString &typname);

//	pgaFactory *GetFactory()
//	{
//		return factory;
//	}
//	bool IsCreatedBy(pgaFactory &f) const
//	{
//		return &f == factory;
//	}
//	bool IsCreatedBy(pgaFactory *f) const
//	{
//		return f == factory;
//	}
//	int GetType() const;
//	int GetMetaType() const;
//	QString GetTypeName() const;
//	QString GetTranslatedTypeName() const;
//	virtual QString GetTranslatedMessage(int kindOfMessage) const;
//	virtual int GetIconId();
//	bool UpdateIcon(ctlTree *browser);

//	virtual void ShowProperties() const {};
//	virtual pgDatabase *GetDatabase() const
//	{
//		return 0;
//	}
//	virtual pgServer *GetServer() const
//	{
//		return 0;
//	}
//	virtual pgSchema *GetSchema() const
//	{
//		return 0;
//	}
//	virtual pgTable *GetTable() const
//	{
//		return 0;
//	}
//	virtual pgaJob *GetJob() const
//	{
//		return 0;
//	}
//	void iSetName(const QString &newVal)
//	{
//		name = newVal;
//	}
//	QString GetName() const
//	{
//		return name;
//	}
//	OID GetOid() const
//	{
//		return oid;
//	}
//	QString GetOidStr() const
//	{
//		return NumToStr(oid) + wxT("::oid");
//	}
//	void iSetOid(const OID newVal)
//	{
//		oid = newVal;
//	}
//	void iSetXid(const OID x)
//	{
//		xid = x;
//	};
//	OID GetXid()
//	{
//		return xid;
//	};
//	QString GetOwner() const
//	{
//		return owner;
//	}
//	void iSetOwner(const QString &newVal)
//	{
//		owner = newVal;
//	}
//	QString GetComment() const
//	{
//		return comment;
//	}
//	void iSetComment(const QString &newVal)
//	{
//		comment = newVal;
//	}
//	QString GetAcl() const
//	{
//		return acl;
//	}
//	void iSetAcl(const QString &newVal)
//	{
//		acl = newVal;
//	}
//	virtual bool GetSystemObject() const
//	{
//		return false;
//	}
//	virtual bool IsCollection() const
//	{
//		return false;
//	}
//	virtual void ShowHint(frmMain *form, bool force) {}

//	void ShowTree(frmMain *form, ctlTree *browser, ctlListView *properties, ctlSQLBox *sqlPane);

//	wxTreeItemId AppendBrowserItem(ctlTree *browser, pgObject *object);

//	virtual QString GetHelpPage(bool forCreate) const;
//	virtual QString GetFullName()
//	{
//		return name;
//	}
//	virtual QString GetIdentifier() const
//	{
//		return name;
//	}
//	virtual QString GetQuotedIdentifier() const
//	{
//		return qtIdent(name);
//	}
//	virtual QString GetDisplayName()
//	{
//		return GetFullName();
//	};
//	void iSetProviders(const QString &newVal)
//	{
//		providers = newVal;
//	}
//	QString GetProviders() const
//	{
//		return providers;
//	}
//	void iSetLabels(const QString &newVal)
//	{
//		labels = newVal;
//	}
//	QString GetLabels() const
//	{
//		return labels;
//	}
//	wxArrayString GetProviderLabelArray();
//	QString GetSeqLabelsSql();

//	virtual wxMenu *GetNewMenu();

//	virtual QString GetSql(ctlTree *browser)
//	{
//		return wxT("");
//	}
//	/*
//	*  Except column level privileges, column will be always an empty
//	*  string in any case
//	*/
//	QString GetGrant(const QString &allPattern, const QString &grantFor = wxT(""), const QString &column = wxT(""));
//	QString GetCommentSql();
//	QString GetOwnerSql(int major, int minor, QString objname = wxEmptyString, QString objtype = wxEmptyString);
//	pgConn *GetConnection() const;

//	virtual void SetDirty()
//	{
//		sql = wxT("");
//		expandedKids = false;
//		needReread = true;
//	}
//	virtual QString GetFullIdentifier() const
//	{
//		return GetName();
//	}
//	virtual QString GetQuotedFullIdentifier() const
//	{
//		return qtIdent(name);
//	}

//	virtual void ShowTreeDetail(ctlTree *browser, frmMain *form = 0, ctlListView *properties = 0, ctlSQLBox *sqlPane = 0) = 0;
//	virtual void ShowStatistics(frmMain *form, ctlListView *statistics);
//	virtual void ShowDependencies(frmMain *form, ctlListView *Dependencies, const QString &where = wxEmptyString);
//	virtual void ShowDependents(frmMain *form, ctlListView *referencedBy, const QString &where = wxEmptyString);
//	virtual pgObject *Refresh(ctlTree *browser, const wxTreeItemId item)
//	{
//		return this;
//	}
//	virtual bool DropObject(wxFrame *frame, ctlTree *browser, bool cascaded = false)
//	{
//		return false;
//	}
//	virtual bool EditObject(wxFrame *frame, ctlTree *browser)
//	{
//		return false;
//	}

//	virtual bool NeedCascadedDrop()
//	{
//		return false;
//	}
//	virtual bool CanCreate()
//	{
//		return false;
//	}
//	virtual bool CanView()
//	{
//		return false;
//	}
//	virtual bool CanEdit()
//	{
//		return false;
//	}
//	virtual bool CanDrop()
//	{
//		return false;
//	}
//	virtual bool CanDropCascaded()
//	{
//		return false;
//	}
//	virtual bool CanMaintenance()
//	{
//		return false;
//	}
//	virtual bool RequireDropConfirm()
//	{
//		return false;
//	}
//	virtual bool WantDummyChild()
//	{
//		return false;
//	}
//	virtual bool CanBackup()
//	{
//		return false;
//	}
//	virtual bool CanBackupGlobals()
//	{
//		return false;
//	}
//	virtual bool CanRestore()
//	{
//		return false;
//	}
//	virtual bool GetCanHint()
//	{
//		return false;
//	}
//	virtual bool HasStats()
//	{
//		return false;
//	}
//	virtual bool HasDepends()
//	{
//		return false;
//	}
//	virtual bool HasReferences()
//	{
//		return false;
//	}

//	QString qtDbString(const QString &str);

//	void SetWindowPtr(dlgProperty *dlgprop);
//	dlgProperty *GetWindowPtr()
//	{
//		return dlg;
//	}

//	bool CheckOpenDialogs(ctlTree *browser, wxTreeItemId node);

//protected:
//	void CreateList3Columns(ctlListView *properties, const QString &left = _("Object"), const QString &middle = _("Owner"), const QString &right = _("Value"));
//	void CreateListColumns(ctlListView *properties, const QString &left = _("Property"), const QString &right = _("Value"));

//	void AppendMenu(wxMenu *menu, int type = -1);
//	virtual void SetContextInfo(frmMain *form) {}

//	bool expandedKids, needReread;
//	QString sql;
//	bool hintShown;
//	pgaFactory *factory;

//private:
//	/*
//	*  Except column level privileges, column will be always an empty
//	*  string in any case
//	*/
//	static void AppendRight(QString &rights, const QString &acl, wxChar c, const wxChar *rightName, const QString &column = wxEmptyString);
//	static QString GetPrivilegeGrant(const QString &allPattern, const QString &acl, const QString &grantObject, const QString &user, const QString &column);
//	void ShowDependency(pgDatabase *db, ctlListView *list, const QString &query, const QString &clsOrder);
//	QString name, owner, schema, comment, acl;
//	int type;
//	OID oid, xid;
//	QString providers, labels;
//	dlgProperty *dlg;

//	friend class pgaFactory;
};

#endif // PGOBJECT_H
