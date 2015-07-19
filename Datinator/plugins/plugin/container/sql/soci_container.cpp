/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <ctime>

#include "plugin/container/sql/soci_container.h"
#include "plugin/sql/gui/db_panel_gui.moc"
#include "plugin/gui/progress.h"
#include "support/db/type_helper.h"

#define MODULENAME "SOCI"

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

void SociContainer::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	SQLContainer::store(oPropertyFile, oPrefix);
}

void SociContainer::restore(QSettings &oPropertyFile, QString const &oPrefix)
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

QString SociContainer::prepareTablename(QString const &oTablename)
{
	return oTablename.toUpper();
}

void SociContainer::handleNotification(Dispatcher<QString> *oSource, QString oTable)
{
	UNUSED(oSource);
	setTablename(prepareTablename(oTable));
}

QString SociContainer::getTablename(void) const
{
	return mTablename;
}

void SociContainer::setTablename(QString const &oTablename)
{
	setSelector("{"+oTablename+"}");
}

QString SociContainer::getQuery(void) const
{
	return mQuery;
}

void SociContainer::setQuery(QString const &oQuery)
{
	mQuery = oQuery;
	setSelector(oQuery);
}

void SociContainer::setSelector(QString const &oId)
{
	Progress prg("Loading from Table ...");

	if(oId.length() > 0 && oId[0] == '{') // If it is a table we select it
	{
		mTablename = oId.mid(1, oId.length()-2);
		selectTable(mTablename, false);
	}
	else
		mTablename = "";

	super::setSelector(oId);
	refreshPreview(selectorToQuery(), mPreviewLimit);
}

QString SociContainer::selectorToQuery(void)
{
	QString q = getSelector();
	if(q.length() == 0)
		return q;

	if(q[0] == '{' && q != "{}" && q.length() > 1)
		q = "select * from "+ q.mid(1, q.length()-2);
	else
		q = mQuery;

	return q;
}

void SociContainer::setSQLLog(QString const &oSQFilename, bool bSQLToFile, bool bFileOnly)
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
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "No output is configured. Please specify either a SQL file or a database target.");
		return false;
	}

	if(mSQLToFile == true && mSQLFilename.length() == 0)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "An SQL file is specified but no filename is given!");
		return false;
	}

	return true;
}

int SociContainer::count(void)
{
	if(!getSession())
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to open database.");
		return false;
	}

	try
	{
		soci::session &session = *getSession();
		unsigned int rows = 0;
		StdString query = "select count(1) from (" + supportlib::string::QtStringToStringT(selectorToQuery()) + ")";
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
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to open database.");
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
		mSQLFile.open(supportlib::string::QtStringToStringT(mSQLFilename), std::ios::out | std::ios::trunc);
		if(!mSQLFile.is_open())
		{
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to open the file: "+mSQLFilename);
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

void SociContainer::refreshTables(QString const &oSelection, bool bNotify)
{
	QList<QString> l = loadTables();
	setTables(l, oSelection, bNotify);
}

bool SociContainer::connect(QString const &oConnectString)
{
	QString con = getConnectString();

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
		mSession = new soci::session(sociFactory(), connectStr.c_str());
		mSociConnectString = connectStr;
		refreshTables();
	}
	catch(std::runtime_error const &e)
	{
		mSociConnectString = "";
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		rc = false;
	}
	prg->setValue(prg->value()+1);

	return rc;
}

bool SociContainer::disconnect(QString const &oConnectString)
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

bool SociContainer::prepareStatement(QString const &oQuery)
{
	try
	{
		mResultSet = new soci::rowset<soci::row>(mSession->prepare << supportlib::string::QtStringToStringT(oQuery));
		mResultIterator = mResultSet->begin();
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to prepare resultset for query ["+oQuery+"]");
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		return false;
	}
	return true;
}

bool SociContainer::isTable(void)
{
	QString t = getSelector();
	if(t.length() == 0)
		return true;

	if(t[0] == '{')
		return true;

	return false;
}

QList<DatabaseColumn *> SociContainer::loadColumns(void)
{
	// When we have an empty table, the query doesn't return any rows and we
	// can't determine the columns from it. So if we have a tablename, we can
	// use this to geet the columns in a database specific way.
	// If this happens with a query, it possibly will fail and return empty.
	if(isTable())
		return readColumnsFromTable(getTablename());

	return readColumnsFromQuery(selectorToQuery(), true);
}

QList<DatabaseColumn *> SociContainer::readColumnsFromTable(QString const &oTablename)
{
	if(oTablename.length() == 0)
		return QList<DatabaseColumn *>();

	return readColumnsFromQuery("select * from " +oTablename+" limit 1", false);
}

QList<DatabaseColumn *> SociContainer::readColumnsFromQuery(QString const &oQuery, bool bLimit)
{
	QList<DatabaseColumn *> columns;
	QString query = oQuery;

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

		std::string q = supportlib::string::QtStringToStringT(query);
		soci::rowset<soci::row> rs = (session.prepare << q);
		soci::rowset<soci::row>::const_iterator it = rs.begin();
		if(it != rs.end())
		{
			soci::row const &row = *it;
			for(std::size_t i = 0; i < row.size(); i++)
			{
				const soci::column_properties &props = row.get_properties(i);

				DatabaseColumn *col = new DatabaseColumn();
				col->setName(supportlib::string::StringTToQtString(props.get_name()));
				col->setPosition(i);
				columns.append(col);

				switch(props.get_data_type())
				{
					case soci::dt_string:
						col->setType(supportlib::db::DataType::type_string);
					break;

					case soci::dt_double:
						col->setType(supportlib::db::DataType::type_decimal);
					break;

					case soci::dt_long_long:
					case soci::dt_integer:
					case soci::dt_unsigned_long_long:
						col->setType(supportlib::db::DataType::type_integer);
					break;

					case soci::dt_date:
					{
						col->setType(supportlib::db::DataType::type_date);
					}
					break;

					default:
					{
						col->setType(supportlib::db::DataType::type_unknown);
					}
				}
			}
		}
	}
	catch(std::runtime_error const &e)
	{
		if(isReader())
		{
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to retrieve columns for query ["+query+"]");
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
		}
	}

	prg->setValue(prg->value()+1);

	return columns;
}

void SociContainer::handleNotification(Dispatcher<SQLPreview::ActionEvent, QVariant, int> *oSource, SQLPreview::ActionEvent nEvent, QVariant oData, int nRows)
{
	UNUSED(oSource);

	Progress prg("Fetch columns...");
	QString query;

	if(nEvent == SQLPreview::ActionEvent::EVENT_SQL_EXECUTE)
	{
		setTablename("");
		query = oData.value<QString>();
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

void SociContainer::refreshPreview(QString const &oQuery, int nLimit)
{
	if(!mPreviewPanel)
		return;

	Progress prg("Updating preview...");
	prg->setLabelText("Updating preview...");

	QList<DatabaseColumn *>columns = getColumns();
	bool ex;
	QList<QList<QString>> rows = fetchRows(columns, oQuery, ex, nLimit);
	mPreviewPanel->setPreview(columns, rows);
}

QList<QList<QString>> SociContainer::fetchRows(QList<DatabaseColumn *> &oColumns, QString const &oQuery, bool &bException, int nCount)
{
	QString t;
	QString sql;
	QList<QList<QString>> rows;
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
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Unable to connect to "+getConnectString());
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
		soci::rowset<soci::row> rs = (mSession->prepare << supportlib::string::QtStringToStringT(sql));
		for(soci::row const &row : rs)
		{
			QApplication::processEvents();
			if(prg->wasCanceled())
				break;

			n++;

			t = "Row "+ QString::number(n) + " ...";
			prg->setLabelText(t);
			if(nCount)
				prg->setValue(prg->value()+1);

			rows.append(fromRow(oColumns, row));

			if(n >= nCount)
				break;
		}
	}
	catch(std::exception &e)
	{
		bException = true;
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, sql);
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}
	n -= nCount;
	prg->setValue(prg->value()+1+n);

	return rows;
}

QList<QString> SociContainer::fromRow(QList<DatabaseColumn *> &oColumns, soci::row const &oRow)
{
	QList<QString> row;
	QString name;

	if(oColumns.size() == 0)
		return row;

	try
	{
		for(std::size_t i = 0; i != oRow.size(); ++i)
		{
			const soci::column_properties &props = oRow.get_properties(i);
			name = supportlib::string::StringTToQtString(props.get_name());
			QString v;
			DatabaseColumn *col = oColumns[i];

			if(oRow.get_indicator(i) != soci::indicator::i_null)
			{
				col->setNull(false);
				switch(props.get_data_type())
				{
					case soci::dt_string:
						v = supportlib::string::StringTToQtString(oRow.get<StdString>(i));
					break;

					case soci::dt_double:
						v = QString::number(oRow.get<double>(i));
					break;

					case soci::dt_integer:
						v = QString::number(oRow.get<int>(i));
					break;

					case soci::dt_long_long:
						v = QString::number(oRow.get<long long>(i));
					break;

					case soci::dt_unsigned_long_long:
						v = QString::number(oRow.get<unsigned long long>(i));
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

			row.append(v);
		}
	}
	catch(std::runtime_error const &e)
	{
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error when converting column ["+name+"]");
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
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
		QString s = "Commit values to database? Press OK to commit, or Cancel to rollback.";
		msgBox.setInformativeText(s);
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
		QString action;
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
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, action+" failed.");
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
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
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Rollback failed.");
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
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

int SociContainer::read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow)
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
		ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
	}

	return -1;
}

bool SociContainer::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!SQLContainer::loadProfile(oProfile, oKey))
		return false;

	if(!getPreviewPanel()->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SociContainer::saveProfile(QSettings &oProfile, QString const &oKey)
{
	SQLContainer::saveProfile(oProfile, oKey);
	getPreviewPanel()->saveProfile(oProfile, oKey);
}

bool SociContainer::prepareOpen(QList<DatabaseColumn *> const &oColumns)
{
	if(!validateOutput())
		return false;

	mInsert = "insert into " + getTablename() + " ( ";
	for(DatabaseColumn * const &mp : oColumns)
	{
		mInsert += mp->getName();
		mInsert += ", ";
	}
	mInsert = mInsert.mid(0, mInsert.length()-2) + " )\n values ( ";

	return true;
}
bool SociContainer::prepareValue(DatabaseColumn *oColumn, QString const &oValue, StdString &oResult)
{
	UNUSED(oColumn);

	StdString s = supportlib::string::QtStringToStringT(oValue);
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

int SociContainer::write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow)
{
	QString values;
	int i = -1;
	StdString query = supportlib::string::QtStringToStringT(mInsert);
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
						ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error converting value for insert:"+oRow[i]);
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
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Error while inserting into "+getTablename());
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, supportlib::string::StringTToQtString(query));
			ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			try
			{
				session.rollback();
			}
			catch(std::runtime_error const &e)
			{
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, "Rollback failed!");
				ErrorMessage(supportlib::logging::LoggingItem::LOG_ERROR, MODULENAME, e.what());
			}
			return -1;
		}
	}

	if(mSQLFile.is_open())
		mSQLFile << query << ";\n";

	return 1;
}

QString SociContainer::createColumnStatement(DatabaseColumn *oColumn)
{
	UNUSED(oColumn);
	return "";
}
