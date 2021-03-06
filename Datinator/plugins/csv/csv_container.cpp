/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>

#include <QtWidgets/QMessageBox>

#include "csv/csv_container.h"
#include "plugin/container/gui/file_panel_gui.moc"
#include "plugin/gui/progress.h"

CSVContainer::CSVContainer(QWidget *oMainWindow)
: super(oMainWindow)
{
	mFilePanel = NULL;
	mRownum = 0;
	setContainername("CSV File");
}

CSVContainer::~CSVContainer(void)
{
	getCSV().close();
}

void CSVContainer::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	super::store(oPropertyFile, "csv/"+oPrefix);
}

void CSVContainer::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	super::restore(oPropertyFile, "csv/"+oPrefix);
}

CSV &CSVContainer::getCSV(void)
{
	return mCSV;
}

CSV const &CSVContainer::getCSV(void) const
{
	return mCSV;
}

bool CSVContainer::begin(void)
{
	CSV &csv = getCSV();
	csv.rewind();
	return super::begin();
}

void CSVContainer::end(void)
{
	CSV &csv = getCSV();
	csv.close();
	super::end();
}

bool CSVContainer::connect(StdString const &oFilename)
{
	StdString fn = oFilename;
	if(fn.length() == 0)
		fn = getConnectString();

	if(fn.length() == 0)
		return false;

	bool rc = super::connect(fn);
	if(rc == false)
		return false;

	Progress prg("Open CSV", "Opening File");
	setRownum(0);
	CSV &csv = getCSV();
	if(fn.length() > 0)
	{
		CSV::ErrorCode rc;
		disconnect();

		csv.setFilename(fn);
		CSV::Openmode md = getOpenmode();
		if((rc = csv.open(md)) != CSV::ErrorCode::CSV_OK)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "CSV", "Unable to open "+fn);
			csv.setFilename("");
			return false;
		}
	}
	else
		return false;

	csv.rewind();

	return true;
}

bool CSVContainer::disconnect(StdString const &oFilename)
{
	UNUSED(oFilename);

	CSV &csv = getCSV();
	csv.close();
	return true;
}

void CSVContainer::releaseColumns(void)
{
	super::releaseColumns();
}
