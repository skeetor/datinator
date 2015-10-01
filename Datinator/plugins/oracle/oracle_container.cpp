/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>

#include "support_qt/db/login_panel/login_panel_model.h"
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

void OracleContainer::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	super::store(oPropertyFile, oPrefix);
	oPropertyFile.remove("oracle");
	LoginPanelModel *model = createLoginPanel()->getModel();
	QList<QString> logins = model->serialize();
	int i = 0;
	for(QString const &login : logins)
	{
		oPropertyFile.setValue("oracle/login_"+QString::number(i), login);
		i++;
	}
}

void OracleContainer::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	super::restore(oPropertyFile, oPrefix);
	LoginPanelModel *model = createLoginPanel()->getModel();
	QList<QString> logins;
	int i = 0;
	do
	{
		QString login = oPropertyFile.value("oracle/login_"+QString::number(i), "").toString();
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

QString OracleContainer::loginToConnectString(DatabaseLogin const &oLogin) const
{
	return supportlib::string::StringTToQtString(oLogin.getUser()+"@"+oLogin.getDatabase()+":"+oLogin.getHostname());
}

DatabaseLogin OracleContainer::connectStringToLogin(QString const &oConnectString) const
{
	DatabaseLogin login;
	if(oConnectString.length() == 0)
		return login;

	QString cs = oConnectString;
	int pos = cs.indexOf('@');
	if(pos != -1)
	{
		login.setUser(supportlib::string::QtStringToStringT(cs.mid(0, pos)));
		cs = cs.mid(pos+1);
	}

	pos = cs.indexOf(':');
	if(pos != -1)
	{
		login.setDatabase(supportlib::string::QtStringToStringT(cs.mid(0, pos)));
		cs = cs.mid(pos+1);
	}

	login.setHost(supportlib::string::QtStringToStringT(cs));

	return login;
}

StdString OracleContainer::sociConnectString(QString const &oContainerConnectString)
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

QList<DatabaseColumn *> OracleContainer::readColumnsFromTable(QString const &oTablename)
{
	QList<DatabaseColumn *> columns;
	if(oTablename.length() == 0 || !getSession())
		return columns;

	StdString query = "select column_id, COLUMN_NAME, DATA_TYPE, NULLABLE from all_tab_columns where table_name = '"+supportlib::string::QtStringToStringT(oTablename)+"' order by column_id asc";

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

			supportlib::db::DataType type = supportlib::db::DataType::type_unknown;
			if(type_t == "VARCHAR2")
				type = supportlib::db::DataType::type_string;
			else if(type_t == "NUMBER")
				type = supportlib::db::DataType::type_integer;
			else if(type_t == "FLOAT")
				type = supportlib::db::DataType::type_decimal;
			else if(type_t == "DATE")
				type = supportlib::db::DataType::type_datetime;
			else if(type_t == "TIMESTAMP")
				type = supportlib::db::DataType::type_datetime_long;

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
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, query.c_str());
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return columns;
}

QList<QString> OracleContainer::loadTables(void)
{
	QList<QString> tables;
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
		prg->setLabelText("Retrieving "+ QString::number(i)+" table name(s) ...");
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
			QString tab = supportlib::string::StringTToQtString(row.get<StdString>(0));
			tables.append(tab);
		}
	}

	return tables;
}

void OracleContainer::handleNotification(Dispatcher<LoginEvent> *oSource, LoginEvent oEvent)
{
	QString connectstring = loginToConnectString(oEvent.getEvent());
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

QString OracleContainer::limitQuery(QString const &oQuery, int nLimit) const
{
	if(nLimit <= 0)
		return oQuery;

	return "select * from ("+oQuery+") where rownum <= " + QString::number(nLimit);
}

bool OracleContainer::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!super::loadProfile(oProfile, oKey))
		return false;

	return true;
}

void OracleContainer::saveProfile(QSettings &oProfile, QString const &oKey)
{
	super::saveProfile(oProfile, oKey);
}

bool OracleContainer::connect(QString const &oConnectString)
{
	if(!super::connect(oConnectString) || !getSession())
		return false;

	soci::session &session = *getSession();

	session << "alter session set global_names = false";

	//session << "alter session set nls_date_format='YYYY.MM.DD HH24:MI:SS'";
	//session << "alter session set nls_timestamp_format='YYYY.MM.DD HH24:MI:SSXFF'";

	return true;
}
