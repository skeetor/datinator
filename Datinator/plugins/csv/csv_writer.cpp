/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QString>

#include <QtWidgets/QMessageBox>

#include "csv/csv_global.h"
#include "csv/csv_writer.h"
#include "plugin/gui/progress.h"

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

QList<DatabaseColumn *> CSVWriter::loadColumns(void)
{
	QList<DatabaseColumn *> cols;

	return cols;
}

void CSVWriter::setFilename(QString const &oFilename)
{
	CSVContainer::setFilename(oFilename);

	if(mConfigPanel == NULL)
		return;

	// If we have a config panel and a new filename is set,
	// we reset it.
	mConfigPanel->reset();
}

CSVWriterConfigPanel *CSVWriter::createContainerConfigPanel(QWidget *oParent)
{
	if(mConfigPanel == NULL)
		mConfigPanel = new CSVWriterConfigPanel(this, oParent);

	return mConfigPanel;
}

bool CSVWriter::prepareOpen(QList<DatabaseColumn *> const &oColumns)
{
	if(!truncateMode())
		return true;

	CSVWriterConfigPanel *p = createContainerConfigPanel(NULL);
	CSV &csv = getCSV();
	csv.setSeparator(p->getSeparator());

	csv.clearColumns();
	std::vector<CSVColumn *>columns;
	for(DatabaseColumn * const &col : oColumns)
	{
		CSVColumn *dbcol = new CSVColumn();

		dbcol->setName(supportlib::string::QtStringToStringT(col->getName()));
		dbcol->setValue(supportlib::string::QtStringToStringT(col->getValue()));
		dbcol->setNativeType(supportlib::string::QtStringToStringT(col->getNativeType()));
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

int CSVWriter::write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow)
{
	QString s;

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
			row.push_back(supportlib::string::QtStringToStringT(oRow[i]));
			null.push_back(false);
		}
	}

	CSV &csv = getCSV();
	CSV::ErrorCode rc;
	if((rc = csv.write(row, null)) != CSV::ErrorCode::CSV_OK)
	{
		QString s = "Error while writing to target. Errorcode: ";
		s += csv.toErrorText(rc);
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, "CSV", s);
		return -1;
	}

	return 1;
}

bool CSVWriter::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!FileContainerBase::loadProfile(oProfile, oKey))
		return false;

	CSVWriterConfigPanel *p = createContainerConfigPanel(NULL);
	QString s = oProfile.value(oKey+"_separator", ";").toString();
	StdChar c1 = ';';
	if(s.length() > 0)
		c1 = s.at(0).toLatin1();
	p->setSeparator(c1);

	s = oProfile.value(oKey+"_bracket", "\"").toString();
	StdChar c2 = 0;
	c1 = '"';
	if(s.length() > 0)
	{
		c1 = s.at(0).toLatin1();
		if(s.length() > 1)
			c2 = s.at(1).toLatin1();
	}

	p->setBracket(c1, c2);

	return true;
}

void CSVWriter::saveProfile(QSettings &oProfile, QString const &oKey)
{
	FileContainerBase::saveProfile(oProfile, oKey);
	CSVWriterConfigPanel *p = createContainerConfigPanel(NULL);
	StdChar c1 = p->getSeparator();
	oProfile.setValue(oKey+"_separator", QString(c1));

	StdChar c2;
	c1 = p->getBracket(c2);
	QString s;
	s += c1;
	s += c2;
	oProfile.setValue(oKey+"_bracket", s);
}

