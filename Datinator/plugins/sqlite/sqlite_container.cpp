/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "support/unicode/unicode_types.h"
#include "sqlite/sqlite_container.h"
#include "container/sql/gui/db_panel_gui.moc"
#include "container/gui/progress.h"

#include "soci.h"
#include "sqlite3/soci-sqlite3.h"

#define MODULENAME "SQLITE3"

SQLiteContainer::SQLiteContainer(QWidget *oMainWindow)
	:	SociContainer(oMainWindow),
		mPathListener(this)
{
	mLoginPanel = NULL;
	setContainername("SQLite 3");
}

SQLiteContainer::~SQLiteContainer(void)
{
}

void SQLiteContainer::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	SociContainer::store(oPropertyFile, oPrefix);
	oPropertyFile.setValue("sqlite3/"+oPrefix+"lastpath", getConnectString());
}

void SQLiteContainer::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	SociContainer::restore(oPropertyFile, oPrefix);
	setConnectString(oPropertyFile.value("sqlite3/"+oPrefix+"lastpath", "").toString());
}

void SQLiteContainer::initPanel(DBPanel *oPanel)
{
	SociContainer::initPanel(oPanel);
	oPanel->setRowStretch(0, 0);
	oPanel->setRowStretch(2, 1);
}

PathSelectPanel *SQLiteContainer::createLoginPanel(void)
{
	if(mLoginPanel == NULL)
	{
		mLoginPanel = new PathSelectPanel();
		mLoginPanel->addPathListener(&mPathListener);
		mLoginPanel->setPathNotify(getConnectString());
		mLoginPanel->setOpenMode(isReader());
	}

	return mLoginPanel;
}

soci::backend_factory const &SQLiteContainer::sociFactory(void)
{
	return soci::sqlite3;
}

StdString SQLiteContainer::sociConnectString(QString const &oContainerConnectString)
{
	return supportlib::string::QtStringToStringT(oContainerConnectString);
}

QString SQLiteContainer::limitQuery(QString const &oQuery, int nLimit) const
{
	if(nLimit <= 0)
		return oQuery;

	return "select * from ("+oQuery+") limit "+QString::number(nLimit);
}

bool SQLiteContainer::tableExists(QString oTablename)
{
	return false;
}

QList<DatabaseColumn *> SQLiteContainer::readColumnsFromTable(QString const &oTablename)
{
	QList<DatabaseColumn *> columns;
	if(oTablename.length() == 0 || !getSession())
		return columns;

	StdString query = "PRAGMA table_info(" +supportlib::string::QtStringToStringT(oTablename)+")";

	try
	{
		int pos = -1;
		soci::session &session = *getSession();
		soci::rowset<soci::row> rs = (session.prepare << query);
		for(soci::row const &row : rs)
		{
			StdString column = row.get<StdString>(1);
			StdString type_t = row.get<StdString>(2);
			StdString null_t = row.get<StdString>(3);
			bool null = true;
			if(null_t.length() > 0)
				null = null_t[0] != '0';

			supportlib::db::DataType type = supportlib::db::DataType::type_unknown;
			if(type_t == "TEXT")
				type = supportlib::db::DataType::type_string;
			else if(type_t == "INTEGER")
				type = supportlib::db::DataType::type_integer;
			else if(type_t == "NUMERIC")
				type = supportlib::db::DataType::type_decimal;
			else if(type_t == "REAL")
				type = supportlib::db::DataType::type_decimal;

			DatabaseColumn *col = new DatabaseColumn();
			col->setPosition(++pos);
			col->setName(supportlib::string::StringTToQtString(column));
			col->setNullable(null);
			col->setType(type);
			columns.append(col);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to load tables from ["+getConnectString()+"]");
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return columns;
}

QList<QString> SQLiteContainer::loadTables(void)
{
	QList<QString> tables;

	Progress prg("Loading tables ...");
	prg->setLabelText("Fetching tables ...");

	soci::session *session_ptr = getSession();
	if(!session_ptr)
		return tables;

	try
	{
		StdString query = "SELECT distinct name FROM sqlite_master WHERE type='table'";
		soci::session &session = *session_ptr;
		int i;
		session << "select count(1) from ("+query+")", soci::into(i);
		prg->setMaximum(prg->maximum()+i);

		soci::rowset<soci::row> rs = (session.prepare << query+" order by name");
		for(soci::row const &row : rs)
		{
			QApplication::processEvents();
			if(prg->wasCanceled())
				break;

			if(!session.got_data())
				continue;

			QString tab = supportlib::string::StringTToQtString(row.get<StdString>(0));
			tables.append(tab);
			prg->setValue(prg->value()+1);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to load tables from ["+getConnectString()+"]");
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return tables;
}

bool SQLiteContainer::loadProfile(QSettings &oProfile, QString const &oKey)
{
	mLoginPanel->setPath(oProfile.value(oKey+"_db_file", "").toString());

	if(!SociContainer::loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SQLiteContainer::saveProfile(QSettings &oProfile, QString const &oKey)
{
	SociContainer::saveProfile(oProfile, oKey);
	oProfile.setValue(oKey+"_db_file", mLoginPanel->getPath());
}
