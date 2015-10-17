/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef CONFIG_LISTENER_H
#define CONFIG_LISTENER_H

#include <vector>

#include "datinator_types.h"
#include <support/patterns/observer.h>
#include <support/db/dbcolumn.h>

class MainFrame;
class MostRecent;

class ConfigListener :
	public Listener<std::vector<DatabaseColumn *> *>,
	public Listener<StdString /* ConnectionString */, StdString /* Selector Id */>
{
public:
	ConfigListener(MainFrame *oMainFrame, MostRecent *oMostRecent);
	virtual ~ConfigListener();

public:
	virtual void handleNotification(Dispatcher<std::vector<DatabaseColumn *> *> *oSource, std::vector<DatabaseColumn *> *oEvent) override;
	virtual void invalidateDispatcher(Dispatcher<std::vector<DatabaseColumn *> *> const *oDispatcher) override;

	virtual void handleNotification(Dispatcher<StdString, StdString> *oSource, StdString oConnectionString, StdString oSelectorId) override;
	virtual void invalidateDispatcher(Dispatcher<StdString, StdString> const *oDispatcher) override;

	MainFrame *getMainFrame(void);
	MostRecent *getMostRecent(void);

private:
	MainFrame *mMainFrame;
	MostRecent *mMostRecent;
};

#endif // CONFIG_LISTENER_H
