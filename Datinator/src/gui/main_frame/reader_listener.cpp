/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "gui/main_frame/main_frame_gui.moc"
#include "gui/main_frame/reader_listener.h"

ReaderListener::ReaderListener(MainFrame *oMainFrame, MostRecent *oMostRecent)
	: ConfigListener(oMainFrame, oMostRecent)
{
}

ReaderListener::~ReaderListener()
{
}

void ReaderListener::handleNotification(Dispatcher<QList<DatabaseColumn *> *> *oSource, QList<DatabaseColumn *> *oColumns)
{
	UNUSED(oSource);

	QList<DatabaseColumn *> ql;
	if(oColumns == NULL)
		oColumns = &ql;

	getMainFrame()->setSourceColumns(reinterpret_cast<IDataContainer *>(oSource), *oColumns);
}
