/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "oracle/oracle_global.h"
#include "oracle/oracle_reader.h"
#include "plugin/sql/gui/db_panel_gui.moc"

#define MODULENAME "ORACLE"

OracleReader::OracleReader(QWidget *oMainWindow)
: OracleContainer(oMainWindow)
{
	setContainerUUID(ORACLE_READER_ID);
}

OracleReader::~OracleReader(void)
{
}

SQLPreviewPanel *OracleReader::createConfigPanel(void)
{
	return getPreviewPanel();
}

void OracleReader::initPanel(DBPanel *oPanel)
{
	OracleContainer::initPanel(oPanel);
	createConfigPanel()->addActionListener(oPanel);
}

bool OracleReader::isReader(void)
{
	return true;
}

void OracleReader::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	OracleContainer::store(oPropertyFile, oPrefix);
}

void OracleReader::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	OracleContainer::restore(oPropertyFile, oPrefix);
}

int OracleReader::read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow)
{
	return SociContainer::read(oColumns, oRow);
}
