/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

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

void ConfigListener::handleNotification(Dispatcher<std::vector<DatabaseColumn *> *> *oSource, std::vector<DatabaseColumn *> *oEvent)
{
	UNUSED(oSource);
	UNUSED(oEvent);
}

void ConfigListener::invalidateDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> const *oDispatcher)
{
	UNUSED(oDispatcher);
}

void ConfigListener::handleNotification(Dispatcher<StdString, StdString> *oSource, StdString oConnectString, StdString oSelector)
{
	IDataContainer *dc = dynamic_cast<IDataContainer *>(oSource);
	mMostRecent->addItem(dc->getContainerUUID(), oConnectString, oSelector);
}

void ConfigListener::invalidateDispatcher(Dispatcher<StdString, StdString> const *oDispatcher)
{
	UNUSED(oDispatcher);
}
