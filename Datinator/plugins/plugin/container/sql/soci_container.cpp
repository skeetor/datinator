/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QString>

#include <ctime>
#include <algorithm>
#include <sstream>

#include "plugin/container/sql/soci_container.h"
#include "plugin/sql/gui/db_panel_gui.moc"
#include "plugin/gui/progress.h"
#include "support/db/type_helper.h"
#include "support/helper/string.h"

#define MODULENAME "SOCI"

Q_DECLARE_METATYPE(StdString)

SociContainer::SociContainer(QWidget *oMainWindow)
: SQLContainer(oMainWindow)
{
	mPreviewLimit = 10;
	mPreviewPanel = NULL;
	mSession = NULL;
	mSQLToFile = false;
	mFileOnly = false;
	mAutoCommitCount = -1;
	mRecordCount = 0;
	mResultSet = NULL;
}

SociContainer::~SociContainer(void)
{
	releaseColumns();
}

void SociContainer::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SQLContainer::store(oPropertyFile, oPrefix);
}

void SociContainer::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	SQLContainer::restore(oPropertyFile, oPrefix);
}

void SociContainer::initPanel(DBPanel *oPanel)
{
	oPanel->addTableSelectListener(this);
}

SQLPreviewPanel *SociContainer::getPreviewPanel(void)
{
	if(mPreviewPanel == NULL)
	{
		mPreviewPanel = new SQLPreviewPanel();
		mPreviewPanel->setPreviewLimit(mPreviewLimit);
		mPreviewPanel->addActionListener(this);
	}

	return mPreviewPanel;
}

StdString SociContainer::prepareTablename(StdString const &oTablename)
{
	StdString s;
	std::transform(oTablename.begin(), oTablename.end(), s.begin(), ::toupper);
	return s;
}

void SociContainer::handleNotification(Dispatcher<StdString> *oSource, StdString oTable)
{
	UNUSED(oSource);
	setTablename(prepareTablename(oTable));
}

StdString SociContainer::getTablename(void) const
{
	return mTablename;
}

void SociContainer::setTablename(StdString const &oTablename)
{
	setSelector("{"+oTablename+"}");
}

StdString SociContainer::getQuery(void) const
{
	return mQuery;
}

void SociContainer::setQuery(StdString const &oQuery)
{
	mQuery = oQuery;
	setSelector(oQuery);
}

void SociContainer::setSelector(StdString const &oId)
{
	Progress prg("Loading from Table ...");

	if(oId.length() > 0 && oId[0] == '{') // If it is a table we select it
	{
		mTablename = oId.substr(1, oId.length()-2);
		selectTable(mTablename, false);
	}
	else
		mTablename = "";

	super::setSelector(oId);
	refreshPreview(selectorToQuery(), mPreviewLimit);
}

StdString SociContainer::selectorToQuery(void)
{
	StdString q = getSelector();
	if(q.length() == 0)
		return q;

	if(q[0] == '{' && q != "{}" && q.length() > 1)
		q = "select * from "+ q.substr(1, q.length()-2);
	else
		q = mQuery;

	return q;
}

void SociContainer::setSQLLog(StdString const &oSQFilename, bool bSQLToFile, bool bFileOnly)
{
	mSQLFilename = oSQFilename;
	mSQLToFile = bSQLToFile;
	mFileOnly = bFileOnly;
}

void SociContainer::setAutocommit(int nAutocommit)
{
	mAutoCommitCount = nAutocommit;
	mRecordCount = 0;
}

bool SociContainer::validateOutput(void)
{
	mSQLFile.close();

	if(mFileOnly == true && mSQLToFile == false)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "No output is configured. Please specify either a SQL file or a database target.");
		return false;
	}

	if(mSQLToFile == true && mSQLFilename.length() == 0)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "An SQL file is specified but no filename is given!");
		return false;
	}

	return true;
}

int SociContainer::count(void)
{
	if(!getSession())
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to open database.");
		return false;
	}

	try
	{
		soci::session &session = *getSession();
		unsigned int rows = 0;
		StdString query = "select count(1) from (" + selectorToQuery() + ")";
		session << query, soci::into(rows);
		return rows;
	}
	catch(std::runtime_error const &e)
	{
		return -1;
	}
}

bool SociContainer::begin(void)
{
	mRecordCount = 0;
	Progress prg("Open database", "Connecting ...");
	prg->setMaximum(prg->maximum()+2);

	if(!getSession())
	{
		prg->setValue(prg->value()+2);
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to open database.");
		return false;
	}
	prg->setValue(prg->value()+1);
	if(isReader())
	{
		if(!prepareStatement(selectorToQuery()))
			return false;
	}
	prg->setValue(prg->value()+1);


	if(mSQLToFile)
	{
		mSQLFile.open(mSQLFilename, std::ios::out | std::ios::trunc);
		if(!mSQLFile.is_open())
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, StdString("Unable to open the file: ")+mSQLFilename);
			return false;
		}
	}

	return true;
}

void SociContainer::end(void)
{
	if(mResultSet)
		delete mResultSet;

	mResultSet = NULL;

	if(mSQLFile.is_open())
		mSQLFile.close();

	//disconnect();
}

soci::session *SociContainer::getSession(void)
{
	if(mSession == NULL)
		connect();

	return mSession;
}

void SociContainer::refreshTables(StdString const &oSelection, bool bNotify)
{
	std::vector<StdString> l = loadTables();
	setTables(l, oSelection, bNotify);
}

bool SociContainer::connect(StdString const &oConnectString)
{
	StdString con = getConnectString();

	if(oConnectString.length() > 0)
	{
		if(con != oConnectString)
		{
			disconnect();
			con = oConnectString;
		}
	}

	if(mSession != NULL) // Same connect string and session already exists.
		return true;

	if(con.length() == 0)
		return false;

	setConnectString(con);

	bool rc = true;
	Progress prg("Connecting to database");
	prg->setLabelText("Connecting to database ...");
	prg->setMaximum(prg->maximum()+1);

	QApplication::processEvents();
	StdString connectStr = sociConnectString(con);

	try
	{
		mSession = new soci::session(sociFactory(), connectStr);
		mSociConnectString = connectStr;
		refreshTables();
	}
	catch(std::runtime_error const &e)
	{
		mSociConnectString = "";
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		rc = false;
	}
	prg->setValue(prg->value()+1);

	return rc;
}

bool SociContainer::disconnect(StdString const &oConnectString)
{
	UNUSED(oConnectString);

	if(mSession == NULL)
		return true;

	clearTables();
	delete mSession;
	mSession = NULL;
	mSociConnectString = "";
	return true;
}

bool SociContainer::prepareStatement(StdString const &oQuery)
{
	try
	{
		mResultSet = new soci::rowset<soci::row>(mSession->prepare << oQuery);
		mResultIterator = mResultSet->begin();
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to prepare resultset for query ["+oQuery+"]");
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		return false;
	}
	return true;
}

bool SociContainer::isTable(void)
{
	StdString t = getSelector();
	if(t.length() == 0)
		return true;

	if(t[0] == '{')
		return true;

	return false;
}

std::vector<DatabaseColumn *> SociContainer::loadColumns(void)
{
	// When we have an empty table, the query doesn't return any rows and we
	// can't determine the columns from it. So if we have a tablename, we can
	// use this to geet the columns in a database specific way.
	// If this happens with a query, it possibly will fail and return empty.
	if(isTable())
		return readColumnsFromTable(getTablename());

	return readColumnsFromQuery(selectorToQuery(), true);
}

std::vector<DatabaseColumn *> SociContainer::readColumnsFromTable(StdString const &oTablename)
{
	if(oTablename.length() == 0)
		return std::vector<DatabaseColumn *>();

	return readColumnsFromQuery("select * from " +oTablename+" limit 1", false);
}

std::vector<DatabaseColumn *> SociContainer::readColumnsFromQuery(StdString const &oQuery, bool bLimit)
{
	std::vector<DatabaseColumn *> columns;
	StdString query = oQuery;

	if(query.length() == 0)
		return columns;

	Progress prg("Retrieve columns ...");
	if(mSession == NULL)
	{
		connect();
		if(mSession == NULL)
			return columns;
	}

	prg->setMaximum(prg->maximum()+1);
	soci::session &session = *mSession;
	prg->setLabelText("Retrieve columns ...");

	try
	{
		if(bLimit)
			query = limitQuery(query, 1);

		std::string q = query;
		soci::rowset<soci::row> rs = (session.prepare << q);
		soci::rowset<soci::row>::const_iterator it = rs.begin();
		if(it != rs.end())
		{
			soci::row const &row = *it;
			for(std::size_t i = 0; i < row.size(); i++)
			{
				const soci::column_properties &props = row.get_properties(i);

				DatabaseColumn *col = new DatabaseColumn();
				col->setName(props.get_name());
				col->setPosition(i);
				columns.push_back(col);

				switch(props.get_data_type())
				{
					case soci::dt_string:
						col->setType(spt::db::DataType::type_string);
					break;

					case soci::dt_double:
						col->setType(spt::db::DataType::type_decimal);
					break;

					case soci::dt_long_long:
					case soci::dt_integer:
					case soci::dt_unsigned_long_long:
						col->setType(spt::db::DataType::type_integer);
					break;

					case soci::dt_date:
					{
						col->setType(spt::db::DataType::type_date);
					}
					break;

					default:
					{
						col->setType(spt::db::DataType::type_unknown);
					}
				}
			}
		}
	}
	catch(std::runtime_error const &e)
	{
		if(isReader())
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to retrieve columns for query ["+query+"]");
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		}
	}

	prg->setValue(prg->value()+1);

	return columns;
}

void SociContainer::handleNotification(Dispatcher<SQLPreview::ActionEvent, QVariant, int> *oSource, SQLPreview::ActionEvent nEvent, QVariant oData, int nRows)
{
	UNUSED(oSource);

	Progress prg("Fetch columns...");
	StdString query;

	if(nEvent == SQLPreview::ActionEvent::EVENT_SQL_EXECUTE)
	{
		setTablename("");
		query = oData.value<StdString>();
		mPreviewLimit = nRows;
		setQuery(query);
		refreshPreview(query, mPreviewLimit);
	}
	else if(nEvent == SQLPreview::ActionEvent::EVENT_SQL_TOGGLE)
	{
		bool enabled = oData.value<bool>();
		if(enabled)
		{
			setTablename("");
			mPreviewLimit = mPreviewPanel->getPreviewLimit();
			query = mPreviewPanel->getQuery();
			setQuery(query);
			refreshPreview(query, mPreviewLimit);
		}
	}
}

void SociContainer::refreshPreview(StdString const &oQuery, int nLimit)
{
	if(!mPreviewPanel)
		return;

	Progress prg("Updating preview...");
	prg->setLabelText("Updating preview...");

	std::vector<DatabaseColumn *>columns = getColumns();
	bool ex;
	std::vector<std::vector<StdString>> rows = fetchRows(columns, oQuery, ex, nLimit);
	mPreviewPanel->setPreview(columns, rows);
}

std::vector<std::vector<StdString>> SociContainer::fetchRows(std::vector<DatabaseColumn *> &oColumns, StdString const &oQuery, bool &bException, int nCount)
{
	StdString t;
	StdString sql;
	std::vector<std::vector<StdString>> rows;
	bException = false;

	if(oQuery.length() == 0)
		return rows;

	Progress prg("Fetching rows ...");
	int n = 0;
	if(mSession == NULL)
	{
		connect();
		if(mSession == NULL)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to connect to "+getConnectString());
			return rows;
		}
	}
	prg->setLabelText("Fetching rows ...");

	if(nCount > 0)
	{
		prg->setMaximum(prg->maximum()+nCount+1);
		sql = limitQuery(oQuery, nCount);
	}
	else
	{
		prg->setMaximum(prg->maximum()+1);
		sql = oQuery;
	}

	try
	{
		soci::rowset<soci::row> rs = (mSession->prepare << sql);
		for(soci::row const &row : rs)
		{
			QApplication::processEvents();
			if(prg->wasCanceled())
				break;

			n++;
			std::stringstream ss;
			ss << "Row "<< n << " ...";

			prg->setLabelText(spt::string::toQt(ss.str()));
			if(nCount)
				prg->setValue(prg->value()+1);

			rows.push_back(fromRow(oColumns, row));

			if(n >= nCount)
				break;
		}
	}
	catch(std::exception &e)
	{
		bException = true;
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, sql);
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}
	n -= nCount;
	prg->setValue(prg->value()+1+n);

	return rows;
}

std::vector<StdString> SociContainer::fromRow(std::vector<DatabaseColumn *> &oColumns, soci::row const &oRow)
{
	std::vector<StdString> row;
	StdString name;

	if(oColumns.size() == 0)
		return row;

	try
	{
		for(std::size_t i = 0; i != oRow.size(); ++i)
		{
			const soci::column_properties &props = oRow.get_properties(i);
			name = props.get_name();
			StdString v;
			DatabaseColumn *col = oColumns[i];

			if(oRow.get_indicator(i) != soci::indicator::i_null)
			{
				std::stringstream ss;

				col->setNull(false);
				switch(props.get_data_type())
				{
					case soci::dt_string:
						v = oRow.get<StdString>(i);
					break;

					case soci::dt_double:
					{
						double n;
						ss << oRow.get<double>(i);
						ss >> n;
						v = n;
					}
					break;

					case soci::dt_integer:
					{
						int n;
						ss << oRow.get<double>(i);
						ss >> n;
						v = n;
					}
					break;

					case soci::dt_long_long:
					{
						long long n;
						ss << oRow.get<double>(i);
						ss >> n;
						v = n;
					}
					break;

					case soci::dt_unsigned_long_long:
					{
						unsigned long long n;
						ss << oRow.get<double>(i);
						ss >> n;
						v = n;
					}
					break;

					case soci::dt_date:
					{
						char tim[255];
						std::tm when = oRow.get<std::tm>(i);
						snprintf(tim, sizeof tim, "%04u.%02u.%02u %02u:%02u:%02u", when.tm_year+1900, when.tm_mon+1, when.tm_mday, when.tm_hour, when.tm_min, when.tm_sec);
						v = tim;
					}
					break;

					default:
					{
						v = "UNKNOWN DATATYPE @ column: "+name;
					}
				}
			}
			else
				col->setNull(true);

			row.push_back(v);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error when converting column ["+name+"]");
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return row;
}

bool SociContainer::commit(void)
{
	bool commit_flag = true;
	if(mAutoCommitCount < 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Commit?");
		msgBox.setInformativeText("Commit values to database? Press OK to commit, or Cancel to rollback.");
		msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		int v = msgBox.result();
		if(v == QMessageBox::Ok)
			commit_flag = true;
		else
			commit_flag = false;
	}

	soci::session *s = getSession();
	if(s)
	{
		StdString action;
		try
		{
			if(commit_flag)
			{
				action = "Commit";
				s->commit();
			}
			else
			{
				action = "Rollback";
				s->rollback();
			}
		}
		catch(std::runtime_error const &e)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, action+" failed.");
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			return false;
		}
	}

	return true;
}


bool SociContainer::rollback(void)
{
	soci::session *s = getSession();
	if(s)
	{
		try
		{
			s->rollback();
		}
		catch(std::runtime_error const &e)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Rollback failed.");
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			return false;
		}
	}

	return true;
}

bool SociContainer::commit(bool bForceCommit)
{
	if(bForceCommit)
	{
		commit();
		return true;
	}

	// If the autocommit is disabled (-1) we never autocommit.
	// If it is at the end, then the call must be performed with an explicit
	// force, because here, we don't know if this is the end.
	if(mAutoCommitCount == -1 || mAutoCommitCount == 0)
		return false;

	mRecordCount++;
	if(mRecordCount >= mAutoCommitCount)
	{
		mRecordCount = 0;
		commit();
		return true;
	}

	return false;
}

int SociContainer::read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow)
{
	try
	{
		if(mResultIterator == mResultSet->end())
			return 0;

		soci::row const &row = *mResultIterator;
		oRow = fromRow(oColumns, row);
		mResultIterator++;

		return 1;
	}
	catch(std::exception &e)
	{
		ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return -1;
}

bool SociContainer::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!SQLContainer::loadProfile(oProfile, oKey))
		return false;

	if(!getPreviewPanel()->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SociContainer::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	SQLContainer::saveProfile(oProfile, oKey);
	getPreviewPanel()->saveProfile(oProfile, oKey);
}

bool SociContainer::prepareOpen(std::vector<DatabaseColumn *> const &oColumns)
{
	if(!validateOutput())
		return false;

	mInsert = "insert into " + getTablename() + " ( ";
	for(DatabaseColumn * const &mp : oColumns)
	{
		mInsert += mp->getName();
		mInsert += ", ";
	}
	mInsert = mInsert.substr(0, mInsert.length()-2) + " )\n values ( ";

	return true;
}
bool SociContainer::prepareValue(DatabaseColumn *oColumn, StdString const &oValue, StdString &oResult)
{
	UNUSED(oColumn);

	StdString s = oValue;
	size_t n = std::count(s.begin(), s.end(), '\'')+1;
	oResult.reserve(oValue.length()+2*n);
	oResult = "";
	n = s.length();
	for(const char &c : s)
	{
		if(c == '\'')
			oResult += "''";
		else
			oResult += c;
	}

	oResult = "'"+oResult+"'";

	return true;
}

int SociContainer::write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow)
{
	StdString values;
	int i = -1;
	StdString query = mInsert;
	StdString vals;

	if(getSession())
	{
		soci::session &session = *getSession();
		try
		{
			for(DatabaseColumn * const &col : oColumns)
			{
				i++;
				StdString rv;
				if(col->isNull())
					rv = "null";
				else
				{
					if(!prepareValue(col, oRow[i], rv))
					{
						ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error converting value for insert:"+oRow[i]);
						return -1;
					}
				}

				vals += rv+", ";
			}

			vals = vals.substr(0, vals.length()-2);
			query += vals + " )";
			if(!mFileOnly)
			{
				session << query;
				commit(false);
			}
		}
		catch(std::runtime_error const &e)
		{
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error while inserting into "+getTablename());
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, query);
			ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			try
			{
				session.rollback();
			}
			catch(std::runtime_error const &e)
			{
				ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, "Rollback failed!");
				ErrorMessage(spt::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			}
			return -1;
		}
	}

	if(mSQLFile.is_open())
		mSQLFile << query << ";\n";

	return 1;
}

StdString SociContainer::createColumnStatement(DatabaseColumn *oColumn)
{
	UNUSED(oColumn);
	return "";
}
