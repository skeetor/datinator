/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef MOST_RECENT_H_INCLUDED
#define MOST_RECENT_H_INCLUDED

#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

#include <support/patterns/observer.h>
#include "gui/main_frame/most_recent_qt.moc"

class MostRecent
	: public Dispatcher<QString /* oMenuReference */, QString /* oConnectString */, QString /* oText */>,
	protected MostRecentQt
{
public:
	typedef struct
	{
		QString mMenuReference;
		QString mConnectString;
		QString mText;
	} ActionItem;

public:
	MostRecent(QMenu *oParent);
	virtual ~MostRecent(void);

	/**
	 * setMenuReference will create a reference for the parent, thus using it
	 * as a single existing menu where items can be added.
	 */
	void setMenuReference(QString const &oMenuReference);

	/**
	 * Append a new menu entry to the specified parent menu. If oBefore is not NULL
	 * the item will be inserted before the specified menu item. This function can be used
	 * if new submenus will be attached to the parent.
	 */
	void addMenu(QString const &oMenuReference, QString const &oText, QAction *oBefore = NULL);
	void addItem(QString const &oMenuReference, QString const &oConnectString, QString const &oText);
	QList<ActionItem> getItems(QString const &oMenuReference);

	void addMenuItemListener(Listener<QString /* oMenuReference */, QString /* oConnectString */, QString /* oText*/> *oListener);
	void removeMenuItemListener(Listener<QString /* oMenuReference */, QString /* oConnectString */, QString /* oText*/> *oListener);

	void setMaxItems(int nMaxItems);
	int getMaxItems(void);

protected:
	void updateMenu(QMenu *oMenu, QList<QAction *> const &oActions);
	void onItemTriggered(QAction *oAction) override;

private:
	QMenu *mParent;
	int mMaxItems;
	int mRefId;
	QMap<QString, QMenu *> mMenus;
	QMap<QString, QList<QAction *>> mMenuItems;
};

#endif // MOST_RECENT_H_INCLUDED
