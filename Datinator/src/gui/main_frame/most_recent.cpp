/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QMenu>

#include "gui/main_frame/most_recent.h"
#include "support/helper/string.h"

Q_DECLARE_METATYPE(MostRecent::ActionItem)

#define MAX_DEFAULT 5

#warning TODO: MostRecent should be a template

MostRecent::MostRecent(QMenu *oParent)
{
	mMaxItems = MAX_DEFAULT;
	mParent = oParent;
	mRefId = 0;
}

MostRecent::~MostRecent(void)
{
}

void MostRecent::setMaxItems(int nMaxItems)
{
	mMaxItems = nMaxItems;
	if(mMaxItems < 1)
		mMaxItems = MAX_DEFAULT;
}

size_t MostRecent::getMaxItems(void)
{
	return mMaxItems;
}

void MostRecent::addMenuItemListener(Listener<StdString, StdString, StdString> *oListener)
{
	Dispatcher<StdString, StdString, StdString>::addListener(oListener);
}

void MostRecent::removeMenuItemListener(Listener<StdString, StdString, StdString> *oListener)
{
	Dispatcher<StdString, StdString, StdString>::removeListener(oListener);
}

void MostRecent::setMenuReference(StdString const &oMenuReference)
{
	mMenus[oMenuReference] = mParent;
	mMenuItems[oMenuReference] = std::vector<QAction *>();
	connectAction(mParent);
}

void MostRecent::addMenu(StdString const &oMenuReference, StdString const &oText, QAction *oBefore)
{
	std::vector<QAction *> l;
	QMenu *m = new QMenu(mParent);
	m->setTitle(spt::string::toQt(oText));
	if(oBefore)
		mParent->insertAction(oBefore, m->menuAction());
	else
		mParent->addAction(m->menuAction());

	connectAction(m);
	mMenus[oMenuReference] = m;
	mMenuItems[oMenuReference] = l;
}

void MostRecent::addItem(StdString const &oMenuReference, StdString const &oConnectString, StdString const &oText)
{
	auto s = spt::string::toQt(oConnectString);
	if(s.length() == 0)
		s = spt::string::toQt(oText);
	else
	{
		if(oText.length() > 0)
			s += ":"+spt::string::toQt(oText);
	}

	if(s.length() == 0)
		return;

	QMenu *m = mMenus[oMenuReference];
	std::vector<QAction *> l = mMenuItems[oMenuReference];

	int i = -1;
	int pos = -1;
	bool found = false;
	for(QAction * &a : l)
	{
		i++;
		if(a->text() == s)
		{
			pos = i;
			found = true;
			break;
		}
	}

	if(pos == 0) // Already on the first slot
		return;

	if(pos == -1)	// not found
	{
		// If the list has reached it's maximum we remove the last item.
		// otherwise it will be simple inserted at the top.
		if(l.size() >= getMaxItems())
			pos = l.size()-1;
	}

	QAction *a = NULL;
	if(pos >= 0)
	{
		a = l[pos];
		l.erase(l.begin() + pos);
		if(!found)
		{
			delete a;
			a = NULL;
		}
	}

	if(a == NULL)
	{
		ActionItem ai;
		ai.mMenuReference = oMenuReference;
		ai.mConnectString = oConnectString;
		ai.mText = oText;

		QVariant v;
		v.setValue(ai);

		a = new QAction(m);
		a->setText(s);
		a->setData(v);
		m->addAction(a);
	}

	l.insert(l.begin(), a);
	mMenuItems[oMenuReference] = l;
	if(l.size() > 1)
		updateMenu(m, l);
}

std::vector<MostRecent::ActionItem> MostRecent::getItems(StdString const &oMenuReference)
{
	std::vector<ActionItem> l;

	for(QAction * const &a : mMenuItems[oMenuReference])
	{
		ActionItem ai = a->data().value<ActionItem>();
		l.push_back(ai);
	}

	return l;
}

void MostRecent::updateMenu(QMenu *oMenu, std::vector<QAction *> const &oActions)
{
	for(QAction * const &a : oActions)
		oMenu->removeAction(a);

	for(QAction * const &a : oActions)
		oMenu->addAction(a);
}

void MostRecent::onItemTriggered(QAction *oAction)
{
	ActionItem ai = oAction->data().value<ActionItem>();
	Dispatcher<StdString, StdString, StdString>::notify(ai.mMenuReference, ai.mConnectString, ai.mText);
}
