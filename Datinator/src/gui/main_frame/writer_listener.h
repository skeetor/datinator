/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef WRITER_LISTENER_H
#define WRITER_LISTENER_H

#include "gui/main_frame/config_listener.h"

class MainFrame;
class MostRecent;

class WriterListener : public ConfigListener
{
public:
	WriterListener(MainFrame *oMainFrame, MostRecent *oMostRecent);
	virtual ~WriterListener();

public:
	void handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oEvent) override;
};

#endif // WRITER_LISTENER_H
