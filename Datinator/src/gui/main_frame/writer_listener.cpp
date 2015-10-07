/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "gui/main_frame/main_frame_gui.moc"
#include "gui/main_frame/writer_listener.h"

WriterListener::WriterListener(MainFrame *oMainFrame, MostRecent *oMostRecent)
	: ConfigListener(oMainFrame, oMostRecent)
{
}

WriterListener::~WriterListener()
{
}

void WriterListener::handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oColumns)
{
	UNUSED(oSource);

	QList<DatabaseColumn *> ql;
	if(oColumns == NULL)
		oColumns = &ql;

	getMainFrame()->setTargetColumns(reinterpret_cast<IDataContainer *>(oSource), *oColumns);
}
