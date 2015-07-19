/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "container/sql/gui/db_panel_gui.moc"
#include "sqlite/sqlite_reader.h"
#include "sqlite/sqlite_global.h"

SQLiteReader::SQLiteReader(QWidget *oMainWindow)
: SQLiteContainer(oMainWindow)
{
	setContainerUUID(SQLITE_READER_ID);
}

SQLiteReader::~SQLiteReader(void)
{
}

SQLPreviewPanel *SQLiteReader::createConfigPanel(void)
{
	return getPreviewPanel();
}

void SQLiteReader::initPanel(DBPanel *oPanel)
{
	SQLiteContainer::initPanel(oPanel);
	createConfigPanel()->addActionListener(oPanel);
}

void SQLiteReader::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	SQLiteContainer::store(oPropertyFile, oPrefix);
}

void SQLiteReader::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	SQLiteContainer::restore(oPropertyFile, oPrefix);
}

int SQLiteReader::read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow)
{
	return SociContainer::read(oColumns, oRow);
}

bool SQLiteReader::isReader(void)
{
	return true;
}

int SQLiteReader::count(void)
{
	return SociContainer::count();
}
