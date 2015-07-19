/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QString>

#include "csv/csv_global.h"
#include "csv/csv_reader.h"
#include "plugin/gui/progress.h"

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

QWidget *CSVReader::getConfigPanel(QWidget *oParentPanel)
{
	QWidget *w = FileContainerBase::getConfigPanel(oParentPanel);
	enlargePanel();
	return w;
}

void CSVReader::updateSampleView(QList<DatabaseColumn *> const &oColumns)
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
	QList<QList<QString>> samples;
	for(int i = 0; i < mPreviewRowCount; i++)
	{
		std::vector<StdString> csv_row;
		std::vector<bool> csv_null;
		n++;
		if((rc = csv.read(csv_row, csv_null)) != CSV::ErrorCode::CSV_OK)
		{
			if(rc == CSV::ErrorCode::CSV_EOF)
				break;

			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULE_TXT, "Error while reading "+getFilename()+ " at line "+QString::number(n) + " Code: "+QString::number(rc)+" "+csv.toErrorText(rc));

			if(rc == CSV::ErrorCode::DATA_COLUMN_MISSMATCH)
			{
				QString s = "Columns dont match with header! ";
				s += csv.toErrorText(rc);
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULE_TXT, s);
			}

			return;
		}

		QList<QString> row;
//		for(DatabaseColumn * const &col : oColumns)
//			row.append(supportlib::string::StringTToQtString(col->getValue()));
		for(StdString &s :csv_row)
			row.append(supportlib::string::StringTToQtString(s));

		samples.append(row);
	}

	mConfigPanel->setRows(samples);
}

QList<DatabaseColumn *> CSVReader::loadColumns(void)
{
	releaseColumns();

	QList<DatabaseColumn *> ql;
	CSV &csv = getCSV();
	if(csv.isOpen())
	{
		csv.rewind();

		std::vector<CSVColumn *> columns = csv.getColumns();
		for(CSVColumn * const &col : columns)
		{
			DatabaseColumn *dbcol = new DatabaseColumn();
			dbcol->setName(supportlib::string::StringTToQtString(col->getName()));
			dbcol->setValue(supportlib::string::StringTToQtString(col->getValue()));
			dbcol->setNativeType(supportlib::string::StringTToQtString(col->getNativeType()));
			dbcol->setType(col->getType());
			dbcol->setPosition(col->getPosition());
			dbcol->setSize(col->getSize());
			dbcol->setPrecision(col->getPrecision());
			dbcol->setNull(col->isNull());
			dbcol->setNullable(col->isNullable());

			ql.append(dbcol);
		}

		updateSampleView(ql);
	}

	return ql;
}

CSVReaderConfigPanel *CSVReader::createContainerConfigPanel(QWidget *oParent)
{
	if(mConfigPanel == NULL)
		mConfigPanel = new CSVReaderConfigPanel(this, oParent);

	return mConfigPanel;
}

int CSVReader::count(void)
{
	unsigned int n;
	if(getCSV().count(n) != CSV::ErrorCode::CSV_OK)
		return -1;

	return n;
}

int CSVReader::read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow)
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

		QString s = "CSV read error at line "+QString::number(getRownum())+" returned: "+QString::number(rc);
		s += " ";
		s += csv.toErrorText(rc);
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULE_TXT, s);
		return -1;
	}

	// Map the columns which were selected to the columns that should go into the output.
	std::size_t n = row.size();
	std::size_t i = -1;
	for(DatabaseColumn * const &col : oColumns)
	{
		i++;
		col->setNull(false);
		QString s;
		if(i < n)
		{
			s = supportlib::string::StringTToQtString(row[i]);
			col->setNull(null[i]);
		}

		oRow.append(s);
	}

	setRownum(getRownum()+1);

	return 1;
}
