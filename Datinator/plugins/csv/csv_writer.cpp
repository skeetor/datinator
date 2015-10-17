/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>

#include <QtWidgets/QMessageBox>

#include "csv/csv_global.h"
#include "csv/csv_writer.h"
#include "plugin/gui/progress.h"
#include <support/helper/string.h>

CSVWriter::CSVWriter(QWidget *oMainWindow)
: CSVContainer(oMainWindow)
{
	mConfigPanel = NULL;
	setContainerUUID(CSV_WRITER_ID);
}

CSVWriter::~CSVWriter(void)
{
}

CSV::Openmode CSVWriter::getOpenmode(void)
{
	CSV::Openmode md;

	md.flags.READ = 1;
	md.flags.WRITE = 1;
	md.flags.CREATE = 1;
	if(truncateMode())
		md.flags.TRUNCATE = 1;

	return md;
}

bool CSVWriter::isReader(void)
{
	return false;
}

bool CSVWriter::canModifyColumns(void) const
{
	return true;
}

bool CSVWriter::canTruncate(void) const
{
	return true;
}

std::vector<DatabaseColumn *> CSVWriter::loadColumns(void)
{
	std::vector<DatabaseColumn *> cols;

	return cols;
}

void CSVWriter::setFilename(StdString const &oFilename)
{
	CSVContainer::setFilename(oFilename);

	if(mConfigPanel == NULL)
		return;

	// If we have a config panel and a new filename is set,
	// we reset it.
	mConfigPanel->reset();
}

CSVWriterConfigPanel *CSVWriter::createContainerConfigPanel(void)
{
	if(mConfigPanel == NULL)
		mConfigPanel = new CSVWriterConfigPanel(this, super::getMainWindow());

	return mConfigPanel;
}

bool CSVWriter::prepareOpen(std::vector<DatabaseColumn *> const &oColumns)
{
	if(!truncateMode())
		return true;

	CSVWriterConfigPanel *p = createContainerConfigPanel();
	CSV &csv = getCSV();
	csv.setSeparator(p->getSeparator());
	StdChar closer;
	StdChar opener = p->getBracket(closer);
	csv.setBracket(opener, closer);

	csv.clearColumns();
	std::vector<CSVColumn *>columns;
	for(DatabaseColumn * const &col : oColumns)
	{
		CSVColumn *dbcol = new CSVColumn();

		dbcol->setName(col->getName());
		dbcol->setValue(col->getValue());
		dbcol->setNativeType(col->getNativeType());
		dbcol->setType(col->getType());
		dbcol->setPosition(col->getPosition());
		dbcol->setSize(col->getSize());
		dbcol->setPrecision(col->getPrecision());
		dbcol->setNull(col->isNull());
		dbcol->setNullable(col->isNullable());

		columns.push_back(dbcol);
	}

	csv.setColumns(columns);
	if(csv.writeHeader() != CSV::ErrorCode::CSV_OK)
		return false;

	return true;
}

int CSVWriter::write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow)
{
	StdString s;

	int i = -1;
	int n = oRow.size();

	std::vector<StdString> row;
	std::vector<bool> null;

	for(DatabaseColumn * const &col : oColumns)
	{
		i++;
		if(col->isNull() || i >= n)
		{
			row.push_back("");
			null.push_back(true);
		}
		else
		{
			row.push_back(oRow[i]);
			null.push_back(false);
		}
	}

	CSV &csv = getCSV();
	CSV::ErrorCode rc;
	if((rc = csv.write(row, null)) != CSV::ErrorCode::CSV_OK)
	{
		StdString s = "Error while writing to target. Errorcode: ";
		s += csv.toErrorText(rc);
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, "CSV", s);
		return -1;
	}

	return 1;
}

bool CSVWriter::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!FileContainerBase::loadProfile(oProfile, oKey))
		return false;

	CSVWriterConfigPanel *p = createContainerConfigPanel();
	StdString s = spt::string::fromQt(oProfile.value(spt::string::toQt(oKey)+"_separator", ";").toString());
	StdChar c1 = ';';
	if(s.length() > 0)
		c1 = s[0];
	p->setSeparator(c1);

	s = spt::string::fromQt(oProfile.value(spt::string::toQt(oKey)+"_bracket", "\"").toString());
	StdChar c2 = 0;
	c1 = '"';
	if(s.length() > 0)
	{
		c1 = s[0];
		if(s.length() > 1)
			c2 = s[1];
	}

	p->setBracket(c1, c2);

	return true;
}

void CSVWriter::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	FileContainerBase::saveProfile(oProfile, oKey);
	CSVWriterConfigPanel *p = createContainerConfigPanel();
	StdChar c1 = p->getSeparator();
	StdString s;
	s = c1;
	oProfile.setValue(spt::string::toQt(oKey)+"_separator", spt::string::toQt(s));

	StdChar c2;
	c1 = p->getBracket(c2);
	s = c1+c2;
	oProfile.setValue(spt::string::toQt(oKey)+"_bracket", spt::string::toQt(s));
}

