#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include <QTreeWidget>

class PGObject;

class ObjectBrowser : public QTreeWidget
{
	Q_OBJECT
public:
	explicit ObjectBrowser(QWidget *parent = nullptr);

	void setEditMenu(const QMenu *menu);
	void addItem(PGObject *item, PGObject *parent = nullptr);

private:
	void showContextMenu(PGObject *object, const QPoint &pos);

signals:
	void signalRefreshItem(PGObject *);
	void signalServerRemoved(int);

private slots:
	void slotCustomContextMenuRequested(const QPoint &point);
	void slotRefreshMenu();

private:
	QMenu *_objectMenu;
};

#endif // OBJECTBROWSER_H
