/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef READER_LISTENER_H
#define READER_LISTENER_H

#include "gui/main_frame/config_listener.h"

class MainFrame;
class MostRecent;

class ReaderListener : public ConfigListener
{
public:
	ReaderListener(MainFrame *oMainFrame, MostRecent *oMostRecent);
	virtual ~ReaderListener();

public:
	void handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oEvent) override;
};

#endif // READER_LISTENER_H
