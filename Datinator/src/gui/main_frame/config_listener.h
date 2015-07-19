/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef CONFIG_LISTENER_H
#define CONFIG_LISTENER_H

#include <QtCore/QList>
#include <support/patterns/observer.h>
#include <support/db/dbcolumn.h>

class MainFrame;
class MostRecent;

class ConfigListener :
	public Listener<QList<DatabaseColumn *> *>,
	public Listener<QString /* ConnectionString */, QString /* Selector Id */>
{
public:
	ConfigListener(MainFrame *oMainFrame, MostRecent *oMostRecent);
	virtual ~ConfigListener();

public:
	virtual void handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oEvent) override;
	virtual void invalidateDispatcher(Dispatcher<QList<DatabaseColumn *> *> const *oDispatcher) override;

	virtual void handleNotification(Dispatcher<QString, QString> *oSource, QString oConnectionString, QString oSelectorId) override;
	virtual void invalidateDispatcher(Dispatcher<QString, QString> const *oDispatcher) override;

	MainFrame *getMainFrame(void);
	MostRecent *getMostRecent(void);

private:
	MainFrame *mMainFrame;
	MostRecent *mMostRecent;
};

#endif // CONFIG_LISTENER_H
