#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include <QTreeWidget>

class PGObject;

class ObjectBrowser : public QTreeWidget
{
	Q_OBJECT
public:
	explicit ObjectBrowser(QWidget *parent = nullptr);
	void addItem(PGObject *item, PGObject *parent = nullptr);

signals:
	void signalRefreshItem(PGObject *);

private:
	void showContextMenu(PGObject *object, const QPoint &pos);

private slots:
	void onCustomContextMenuRequested(const QPoint &point);
	void slotItemNeedRefreshing(PGObject *);
};

#endif // OBJECTBROWSER_H
