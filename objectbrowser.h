#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include <QTreeWidget>

class PGObject;

class ObjectBrowser : public QTreeWidget
{
	Q_OBJECT
public:
	explicit ObjectBrowser(QWidget *parent = nullptr);
	void addTopItem(PGObject *obj);

private:
	void showContextMenu(PGObject *object, const QPoint &pos);

private slots:
	void onCustomContextMenuRequested(const QPoint &point);
};

#endif // OBJECTBROWSER_H
