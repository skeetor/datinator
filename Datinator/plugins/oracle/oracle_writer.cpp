/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "oracle/oracle_global.h"
#include "soci.h"
#include "oracle/oracle_writer.h"
#include "oracle/gui/oracle_writer_gui.moc"
#include "container/gui/progress.h"

#include "support/db/dbcolumn.h"

#define MODULENAME "ORACLE"

OracleWriter::OracleWriter(QWidget *oMainWindow)
: OracleContainer(oMainWindow)
{
	mConfigPanel = NULL;
	setContainerUUID(ORACLE_WRITER_ID);
	mAllowTruncate = false;
	mAllowModifyColumns = false;
}

OracleWriter::~OracleWriter(void)
{
}

bool OracleWriter::isReader(void)
{
	return false;
}
/**
 * For our puporse we always return false here. For a generic
 * database, this is dependentant on the current login and
 * schema rights.
 */
bool OracleWriter::canModifyColumns(void) const
{
	if(mAllowModifyColumns)
		return true;

	return false;
}

/**
 * For our puporse we always return false here. For a generic
 * database, this is dependentant on the current login and
 * schema rights.
 */
bool OracleWriter::canTruncate(void) const
{
	if(mAllowTruncate)
		return true;

	return false;
}

bool OracleWriter::defaultTruncate(void) const
{
	return false;
}

void OracleWriter::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	OracleContainer::store(oPropertyFile, oPrefix);
	oPropertyFile.setValue("oracle/modify_columns", mAllowModifyColumns);
	oPropertyFile.setValue("oracle/allow_truncate", mAllowTruncate);
}

void OracleWriter::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	OracleContainer::restore(oPropertyFile, oPrefix);
	mAllowModifyColumns = oPropertyFile.value("oracle/modify_columns", "false").toBool();
	mAllowTruncate = oPropertyFile.value("oracle/allow_truncate", "false").toBool();
}

OracleWriterPanel *OracleWriter::createConfigPanel(void)
{
	if(mConfigPanel == NULL)
		mConfigPanel = new OracleWriterPanel();

	return mConfigPanel;
}

void OracleWriter::commit(void)
{
	SociContainer::commit();
}

void OracleWriter::rollback(void)
{
	SociContainer::rollback();
}

bool OracleWriter::prepareOpen(QList<DatabaseColumn *> const &oColumns)
{
	setSQLLog(createConfigPanel()->getPath(), createConfigPanel()->getExportSQL(), createConfigPanel()->getExportSQLOnly());
	if(!SociContainer::prepareOpen(oColumns))
		return false;

	OracleWriterPanel *wp = createConfigPanel();
	setAutocommit(wp->getAutoCommitCount());

	QString table = getTablename();
	if(table.length() == 0)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "No target table selected!");
		return false;
	}

	if(getSession() == NULL)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Database session not initialized!");
		return false;
	}

	if(mAllowTruncate)
	{
#warning TODO: Implement truncate
	}

	if(mAllowModifyColumns)
	{
#warning TODO: Implement modify columns
	}

	return true;
}

QString OracleWriter::getDateFormatString(QString const &oValue, bool bTimestamp) const
{
	QString pattern = "0000-00-00 00:00:00";

	if(oValue.length() == 10)
		return "YYYY.MM.DD";

	if(oValue.length() >= 19 || bTimestamp == false)
		return "YYYY.MM.DD HH24:MI:SS";

	pattern += ".";
	QString format = "YYYY.MM.DD HH24:MI:SS.";

	QString v = oValue.mid(pattern.length(), oValue.length());
	for(int i = 0; i < v.length(); i++)
	{
//		int c = v[i].toAscii();
		if(v[i] >= '0' && v[i] <= '9')
		{
			format += 'F';
			continue;
		}
		break;
	}

	return format;
}

bool OracleWriter::prepareValue(DatabaseColumn *oColumn, QString const &oValue, StdString &oOutValue)
{
	oOutValue = "";
	if(oColumn->isNull())
	{
		oOutValue = "null";
		return true;
	}

	switch(oColumn->getType())
	{
		case supportlib::db::type_datetime_long:
		case supportlib::db::type_datetime:
		case supportlib::db::type_date:
		{
			bool timestamp = false;
			QString dtfmt = "to_date";
			if(oColumn->getNativeType() == "TIMESTAMP")
			{
				timestamp = true;
				dtfmt = "to_timestamp";
			}

			QString dformat = getDateFormatString(oValue, timestamp);
			oOutValue = supportlib::string::QtStringToStringT(dtfmt+"('"+oValue+"', '"+dformat+"')");
			return true;
		}
		break;

		/*
		case supportlib::db::type_decimal:
		case supportlib::db::type_integer:
		case supportlib::db::type_ip:
		case supportlib::db::type_string:
		*/
		default:
		break;
	}

	return SociContainer::prepareValue(oColumn, oValue, oOutValue);
}

int OracleWriter::write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow)
{
	return SociContainer::write(oColumns, oRow);
}

bool OracleWriter::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!OracleContainer::loadProfile(oProfile, oKey))
		return false;

	if(!mConfigPanel->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void OracleWriter::saveProfile(QSettings &oProfile, QString const &oKey)
{
	OracleContainer::saveProfile(oProfile, oKey);
	mConfigPanel->saveProfile(oProfile, oKey);
}
