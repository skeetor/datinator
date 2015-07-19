/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QMenu>

#include "gui/main_frame/most_recent.h"

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

int MostRecent::getMaxItems(void)
{
	return mMaxItems;
}

void MostRecent::addMenuItemListener(Listener<QString, QString, QString> *oListener)
{
	Dispatcher<QString, QString, QString>::addListener(oListener);
}

void MostRecent::removeMenuItemListener(Listener<QString, QString, QString> *oListener)
{
	Dispatcher<QString, QString, QString>::removeListener(oListener);
}

void MostRecent::setMenuReference(QString const &oMenuReference)
{
	mMenus[oMenuReference] = mParent;
	mMenuItems[oMenuReference] = QList<QAction *>();
	connectAction(mParent);
}

void MostRecent::addMenu(QString const &oMenuReference, QString const &oText, QAction *oBefore)
{
	QList<QAction *> l;
	QMenu *m = new QMenu(mParent);
	m->setTitle(oText);
	if(oBefore)
		mParent->insertAction(oBefore, m->menuAction());
	else
		mParent->addAction(m->menuAction());

	connectAction(m);
	mMenus[oMenuReference] = m;
	mMenuItems[oMenuReference] = l;
}

void MostRecent::addItem(QString const &oMenuReference, QString const &oConnectString, QString const &oText)
{
	QString s = oConnectString;
	if(s.length() == 0)
		s = oText;
	else
	{
		if(oText.length() > 0)
			s += ":"+oText;
	}

	if(s.length() == 0)
		return;

	QMenu *m = mMenus[oMenuReference];
	QList<QAction *> l = mMenuItems[oMenuReference];

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
		a = l.takeAt(pos);
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

	l.insert(0, a);
	mMenuItems[oMenuReference] = l;
	if(l.size() > 1)
		updateMenu(m, l);
}

QList<MostRecent::ActionItem> MostRecent::getItems(QString const &oMenuReference)
{
	QList<ActionItem> l;

	for(QAction * const &a : mMenuItems[oMenuReference])
	{
		ActionItem ai = a->data().value<ActionItem>();
		l.append(ai);
	}

	return l;
}

void MostRecent::updateMenu(QMenu *oMenu, QList<QAction *> const &oActions)
{
	for(QAction * const &a : oActions)
		oMenu->removeAction(a);

	for(QAction * const &a : oActions)
		oMenu->addAction(a);
}

void MostRecent::onItemTriggered(QAction *oAction)
{
	ActionItem ai = oAction->data().value<ActionItem>();
	Dispatcher<QString, QString, QString>::notify(ai.mMenuReference, ai.mConnectString, ai.mText);
}
