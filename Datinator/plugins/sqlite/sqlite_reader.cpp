/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "plugin/sql/gui/db_panel_gui.moc"
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

void SQLiteReader::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SQLiteContainer::store(oPropertyFile, oPrefix);
}

void SQLiteReader::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SQLiteContainer::restore(oPropertyFile, oPrefix);
}

int SQLiteReader::read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow)
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
