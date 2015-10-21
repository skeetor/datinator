/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "csv/csv_global.h"
#include "csv/csv_reader.h"
#include "plugin/gui/progress.h"
#include "plugin/container/gui/file_panel_gui.moc"

#include <support/helper/string.h>

#define MODULE_TXT "CSV"

CSVReader::CSVReader(QWidget *oMainWindow)
: CSVContainer(oMainWindow)
{
	mConfigPanel = NULL;
	mPreviewRowCount = CSV_PREVIEW_ROWS;
	setContainerUUID(CSV_READER_ID);
}

CSVReader::~CSVReader(void)
{
}

CSV::Openmode CSVReader::getOpenmode(void)
{
	CSV::Openmode md;

	md.flags.READ = 1;
	md.flags.WRITE = 1;

	return md;
}

bool CSVReader::isReader(void)
{
	return true;
}

void CSVReader::updateSampleView(std::vector<DatabaseColumn *> const &oColumns)
{
	if(!mConfigPanel)
		return;

	CSV::ErrorCode rc;
	CSV &csv = getCSV();
	mConfigPanel->setColumns(oColumns);
	if(!csv.isOpen())
		return;

	// Load the preview from the file
	int n = 0;
	csv.rewind();
	std::vector<std::vector<StdString>> samples;
	for(int i = 0; i < mPreviewRowCount; i++)
	{
		std::vector<StdString> csv_row;
		std::vector<bool> csv_null;
		n++;
		if((rc = csv.read(csv_row, csv_null)) != CSV::ErrorCode::CSV_OK)
		{
			if(rc == CSV::ErrorCode::CSV_EOF)
				break;

			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULE_TXT, "Error while reading "+getFilename()+ " at line "+spt::string::toString(n) + " Code: "+spt::string::toString(rc)+" "+csv.toErrorText(rc));

			if(rc == CSV::ErrorCode::DATA_COLUMN_MISSMATCH)
			{
				StdString s = "Columns dont match with header! ";
				s += csv.toErrorText(rc);
				ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULE_TXT, s);
			}

			return;
		}

		std::vector<StdString> row;
//		for(DatabaseColumn * const &col : oColumns)
//			row.append(spt::string::StringTToQtString(col->getValue()));
		for(StdString &s :csv_row)
			row.push_back(s);

		samples.push_back(row);
	}

	mConfigPanel->setRows(samples);
}

std::vector<DatabaseColumn *> CSVReader::loadColumns(void)
{
	releaseColumns();

	std::vector<DatabaseColumn *> ql;
	CSV &csv = getCSV();
	if(csv.isOpen())
	{
		csv.rewind();

		std::vector<CSVColumn *> columns = csv.getColumns();
		for(CSVColumn * const &col : columns)
		{
			DatabaseColumn *dbcol = new DatabaseColumn();
			dbcol->setName(col->getName());
			dbcol->setValue(col->getValue());
			dbcol->setNativeType(col->getNativeType());
			dbcol->setType(col->getType());
			dbcol->setPosition(col->getPosition());
			dbcol->setSize(col->getSize());
			dbcol->setPrecision(col->getPrecision());
			dbcol->setNull(col->isNull());
			dbcol->setNullable(col->isNullable());

			ql.push_back(dbcol);
		}

		updateSampleView(ql);
	}

	return ql;
}

FilePanel *CSVReader::getFilePanel(void)
{
	FilePanel *fp = super::getFilePanel();
	if(mConfigPanel == NULL)
		getReaderConfigPanel();

	return fp;
}

CSVReaderConfigPanel *CSVReader::getReaderConfigPanel(void)
{
	if(mConfigPanel == NULL)
	{
		FilePanel *fp = super::getFilePanel();
		mConfigPanel = new CSVReaderConfigPanel(getMainWindow());
		fp->addConfigPanel(mConfigPanel);
	}

	return mConfigPanel;
}

int CSVReader::count(void)
{
	unsigned int n;
	if(getCSV().count(n) != CSV::ErrorCode::CSV_OK)
		return -1;

	return n;
}

int CSVReader::read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow)
{
	oRow.clear();

	CSV &csv = getCSV();
	std::vector<StdString> row;
	std::vector<bool> null;
	CSV::ErrorCode rc;

	// read next line from the csv file
	if((rc = csv.read(row, null)) != CSV::ErrorCode::CSV_OK)
	{
		if(rc == CSV::ErrorCode::CSV_EOF)
			return 0;

		StdString s = "CSV read error at line "+spt::string::toString(getRownum())+" returned: "+spt::string::toString(rc);
		s += " ";
		s += csv.toErrorText(rc);
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULE_TXT, s);
		return -1;
	}

	// Map the columns which were selected to the columns that should go into the output.
	std::size_t n = row.size();
	std::size_t i = -1;
	for(DatabaseColumn * const &col : oColumns)
	{
		i++;
		col->setNull(false);
		StdString s;
		if(i < n)
		{
			s = row[i];
			col->setNull(null[i]);
		}

		oRow.push_back(s);
	}

	setRownum(getRownum()+1);

	return 1;
}
