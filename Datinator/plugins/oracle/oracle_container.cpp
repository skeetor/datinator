/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include <support/helper/string.h>
#include <support_qt/db/login_panel/login_panel_model.h>

#include "oracle/oracle_container.h"
#include "plugin/sql/gui/db_panel_gui.moc"

#include "soci.h"
#include "oracle/soci-oracle.h"
#include "plugin/gui/progress.h"

#define MODULENAME "ORACLE"

OracleContainer::OracleContainer(QWidget *oMainWindow)
: SociContainer(oMainWindow)
{
	mLoginPanel = NULL;
	setContainername("Oracle");
}

OracleContainer::~OracleContainer(void)
{
	disconnect();
}

void OracleContainer::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	super::store(oPropertyFile, oPrefix);
	oPropertyFile.remove("oracle");
	LoginPanelModel *model = createLoginPanel()->getModel();
	std::vector<StdString> logins = model->serialize();
	int i = 0;
	for(StdString const &login : logins)
	{
		oPropertyFile.setValue(spt::string::toQt("oracle/login_"+spt::string::toString(i)), spt::string::toQt(login));
		i++;
	}
}

void OracleContainer::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	super::restore(oPropertyFile, oPrefix);
	LoginPanelModel *model = createLoginPanel()->getModel();
	std::vector<StdString> logins;
	int i = 0;
	do
	{
		StdString login = spt::string::fromQt(oPropertyFile.value(spt::string::toQt("oracle/login_"+spt::string::toString(i)), "").toString());
		if(login.length() == 0)
			break;
		logins.push_back(login);
		i++;
	}while(1);

	model->deserialize(logins);
}

void OracleContainer::initPanel(DBPanel *oPanel)
{
	super::initPanel(oPanel);
	oPanel->setRowStretch(0, 1);
	oPanel->setRowStretch(2, 3);
}

DatabaseLoginPanel *OracleContainer::createLoginPanel(void)
{
	if(mLoginPanel == NULL)
	{
		mLoginPanel = new DatabaseLoginPanel(true);
		mLoginPanel->setReadOnly(false);
		mLoginPanel->addLoginListener(this);
	}

	return mLoginPanel;
}

soci::backend_factory const &OracleContainer::sociFactory(void)
{
	return soci::oracle;
}

StdString OracleContainer::loginToConnectString(DatabaseLogin const &oLogin) const
{
	return oLogin.getUser()+"@"+oLogin.getDatabase()+":"+oLogin.getHostname();
}

DatabaseLogin OracleContainer::connectStringToLogin(StdString const &oConnectString) const
{
	DatabaseLogin login;
	if(oConnectString.length() == 0)
		return login;

	StdString cs = oConnectString;
	size_t pos = cs.find('@');
	if(pos != std::string::npos)
	{
		login.setUser(cs.substr(0, pos));
		cs = cs.substr(pos+1);
	}

	pos = cs.find(':');
	if(pos != std::string::npos)
	{
		login.setDatabase(cs.substr(0, pos));
		cs = cs.substr(pos+1);
	}

	login.setHost(cs);

	return login;
}

StdString OracleContainer::sociConnectString(StdString const &oContainerConnectString)
{
	// If the conectstring is empty, we use the current login.
	if(oContainerConnectString.length() > 0)
	{
		DatabaseLogin login = connectStringToLogin(oContainerConnectString);

		// Get the data for the login to get the password.
		int i = createLoginPanel()->find(login);
		if(i != -1)
		{
			mLogin = createLoginPanel()->getLogin(i);
			createLoginPanel()->select(mLogin, false);
		}
	}

	StdString s;
	s = "service="+mLogin.getDatabase();
	s += " user="+mLogin.getUser();
	s += " password="+mLogin.getClearPassword();

	return s;
}

std::vector<DatabaseColumn *> OracleContainer::readColumnsFromTable(StdString const &oTablename)
{
	std::vector<DatabaseColumn *> columns;
	if(oTablename.length() == 0 || !getSession())
		return columns;

	StdString query = "select column_id, COLUMN_NAME, DATA_TYPE, NULLABLE from all_tab_columns where table_name = '"+oTablename+"' order by column_id asc";

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
				null = null_t[0] != 'N';

			spt::db::DataType type = spt::db::DataType::type_unknown;
			if(type_t == "VARCHAR2")
				type = spt::db::DataType::type_string;
			else if(type_t == "NUMBER")
				type = spt::db::DataType::type_integer;
			else if(type_t == "FLOAT")
				type = spt::db::DataType::type_decimal;
			else if(type_t == "DATE")
				type = spt::db::DataType::type_datetime;
			else if(type_t == "TIMESTAMP")
				type = spt::db::DataType::type_datetime_long;

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
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, query);
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return columns;
}

std::vector<StdString> OracleContainer::loadTables(void)
{
	std::vector<StdString> tables;
	if(mLogin.getUser().size() > 0 && mLogin.getDatabase().size() > 0)
	{
		Progress prg("Loading tables ...");
		if(!getSession())
			return tables;

		prg->setLabelText("Fetching table count ...");
		soci::session &session = *getSession();
		int i;

		session << "select count(1) from all_tables", soci::into(i);
		prg->setMaximum(prg->maximum()+i);
		prg->setLabelText(spt::string::toQt("Retrieving "+spt::string::toString(i)+" table name(s) ..."));
		prg->setValue(prg->value()+1);

		soci::rowset<soci::row> rs = (session.prepare << "select distinct table_name from all_tables order by table_name");
		for(soci::row const &row : rs)
		{
			QApplication::processEvents();
			if(prg->wasCanceled())
				break;

			prg->setValue(prg->value()+1);
			if(!session.got_data())
				continue;
			StdString tab = row.get<StdString>(0);
			tables.push_back(tab);
		}
	}

	return tables;
}

void OracleContainer::handleNotification(Dispatcher<LoginEvent> *oSource, LoginEvent oEvent)
{
	UNUSED(oSource);

	StdString connectstring = loginToConnectString(oEvent.getEvent());
	Progress prg("Switching to database ... ["+connectstring + "]", "Connecting ...");
	if(!oEvent.isActivated() || mLogin != oEvent.getEvent())
	{
		disconnect(connectstring);
		if(!oEvent.isActivated())
		{
			DatabaseLogin l;
			mLogin = l;
			connectstring = "";
		}
	}

	setConnectString(connectstring);
	connect();
}

StdString OracleContainer::limitQuery(StdString const &oQuery, int nLimit) const
{
	if(nLimit <= 0)
		return oQuery;

	return "select * from ("+oQuery+") where rownum <= " + spt::string::toString(nLimit);
}

bool OracleContainer::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!super::loadProfile(oProfile, oKey))
		return false;

	return true;
}

void OracleContainer::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	super::saveProfile(oProfile, oKey);
}

bool OracleContainer::connect(StdString const &oConnectString)
{
	if(!super::connect(oConnectString) || !getSession())
		return false;

	soci::session &session = *getSession();

	session << "alter session set global_names = false";

	//session << "alter session set nls_date_format='YYYY.MM.DD HH24:MI:SS'";
	//session << "alter session set nls_timestamp_format='YYYY.MM.DD HH24:MI:SSXFF'";

	return true;
}
