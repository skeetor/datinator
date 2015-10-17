/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef MOST_RECENT_H_INCLUDED
#define MOST_RECENT_H_INCLUDED

#include <QtCore/QSettings>
#include <vector>
#include <map>

#include <QtCore/QVariant>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

#include "datinator_types.h"
#include <support/patterns/observer.h>
#include "gui/main_frame/most_recent_qt.moc"

class MostRecent
	: public Dispatcher<StdString /* oMenuReference */, StdString /* oConnectString */, StdString /* oText */>,
	protected MostRecentQt
{
public:
	typedef struct
	{
		StdString mMenuReference;
		StdString mConnectString;
		StdString mText;
	} ActionItem;

public:
	MostRecent(QMenu *oParent);
	virtual ~MostRecent(void);

	/**
	 * setMenuReference will create a reference for the parent, thus using it
	 * as a single existing menu where items can be added.
	 */
	void setMenuReference(StdString const &oMenuReference);

	/**
	 * Append a new menu entry to the specified parent menu. If oBefore is not NULL
	 * the item will be inserted before the specified menu item. This function can be used
	 * if new submenus will be attached to the parent.
	 */
	void addMenu(StdString const &oMenuReference, StdString const &oText, QAction *oBefore = NULL);
	void addItem(StdString const &oMenuReference, StdString const &oConnectString, StdString const &oText);
	std::vector<ActionItem> getItems(StdString const &oMenuReference);

	void addMenuItemListener(Listener<StdString /* oMenuReference */, StdString /* oConnectString */, StdString /* oText*/> *oListener);
	void removeMenuItemListener(Listener<StdString /* oMenuReference */, StdString /* oConnectString */, StdString /* oText*/> *oListener);

	void setMaxItems(int nMaxItems);
	size_t getMaxItems(void);

protected:
	void updateMenu(QMenu *oMenu, std::vector<QAction *> const &oActions);
	void onItemTriggered(QAction *oAction) override;

private:
	QMenu *mParent;
	size_t mMaxItems;
	size_t mRefId;
	std::map<StdString, QMenu *> mMenus;
	std::map<StdString, std::vector<QAction *>> mMenuItems;
};

#endif // MOST_RECENT_H_INCLUDED
