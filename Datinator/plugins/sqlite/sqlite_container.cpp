/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "support/helper/string.h"

#include "sqlite/sqlite_container.h"

#include "plugin/gui/progress.h"
#include "plugin/sql/gui/db_panel_gui.moc"

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

void SQLiteContainer::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SociContainer::store(oPropertyFile, oPrefix);
	oPropertyFile.setValue(spt::string::toQt("sqlite3/"+oPrefix+"lastpath"), spt::string::toQt(getConnectString()));
}

void SQLiteContainer::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SociContainer::restore(oPropertyFile, oPrefix);
	setConnectString(spt::string::fromQt(oPropertyFile.value(spt::string::toQt("sqlite3/"+oPrefix+"lastpath"), "").toString()));
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

StdString SQLiteContainer::sociConnectString(StdString const &oContainerConnectString)
{
	return oContainerConnectString;
}

StdString SQLiteContainer::limitQuery(StdString const &oQuery, int nLimit) const
{
	if(nLimit <= 0)
		return oQuery;

	return "select * from ("+oQuery+") limit "+spt::string::toString(nLimit);
}

bool SQLiteContainer::tableExists(StdString oTablename)
{
	UNUSED(oTablename);

	return false;
}

std::vector<DatabaseColumn *> SQLiteContainer::readColumnsFromTable(StdString const &oTablename)
{
	std::vector<DatabaseColumn *> columns;
	if(oTablename.length() == 0 || !getSession())
		return columns;

	StdString query = "PRAGMA table_info(" +oTablename+")";

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

			spt::db::DataType type = spt::db::DataType::type_unknown;
			if(type_t == "TEXT")
				type = spt::db::DataType::type_string;
			else if(type_t == "INTEGER")
				type = spt::db::DataType::type_integer;
			else if(type_t == "NUMERIC")
				type = spt::db::DataType::type_decimal;
			else if(type_t == "REAL")
				type = spt::db::DataType::type_decimal;

			DatabaseColumn *col = new DatabaseColumn();
			col->setPosition(++pos);
			col->setName(column);
			col->setNullable(null);
			col->setType(type);
			columns.push_back(col);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to load tables from ["+getConnectString()+"]");
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return columns;
}

std::vector<StdString> SQLiteContainer::loadTables(void)
{
	std::vector<StdString> tables;

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

			StdString tab = row.get<StdString>(0);
			tables.push_back(tab);
			prg->setValue(prg->value()+1);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to load tables from ["+getConnectString()+"]");
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return tables;
}

bool SQLiteContainer::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto key = spt::string::toQt(oKey);

	mLoginPanel->setPath(spt::string::fromQt(oProfile.value(key+"_db_file", "").toString()));

	if(!SociContainer::loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SQLiteContainer::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	auto key = spt::string::toQt(oKey);

	SociContainer::saveProfile(oProfile, oKey);
	oProfile.setValue(key+"_db_file", spt::string::toQt(mLoginPanel->getPath()));
}
