/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QString>

#include "plugins/idata_container.h"
#include "gui/main_frame/config_listener.h"
#include "gui/main_frame/most_recent.h"

ConfigListener::ConfigListener(MainFrame *oMainFrame, MostRecent *oMostRecent)
{
	mMainFrame = oMainFrame;
	mMostRecent = oMostRecent;
}

ConfigListener::~ConfigListener()
{
}

MainFrame *ConfigListener::getMainFrame(void)
{
	return mMainFrame;
}

MostRecent *ConfigListener::getMostRecent(void)
{
	return mMostRecent;
}

void ConfigListener::handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oEvent)
{
	UNUSED(oSource);
	UNUSED(oEvent);
}

void ConfigListener::invalidateDispatcher(Dispatcher<QList<DatabaseColumn *> *> const *oDispatcher)
{
	UNUSED(oDispatcher);
}

void ConfigListener::handleNotification(Dispatcher<QString, QString> *oSource, QString oConnectString, QString oSelector)
{
	IDataContainer *dc = dynamic_cast<IDataContainer *>(oSource);
	mMostRecent->addItem(dc->getContainerUUID(), oConnectString, oSelector);
}

void ConfigListener::invalidateDispatcher(Dispatcher<QString, QString> const *oDispatcher)
{
	UNUSED(oDispatcher);
}
