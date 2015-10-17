/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _SOCI_CONTAINER_H_INCLUDED
#define _SOCI_CONTAINER_H_INCLUDED

#include "datinator_types.h"
#include "plugin/plugin_dll_api.h"
#include "plugin/container/sql/sql_container.h"
#include "plugin/sql/gui/sql_preview_gui.moc"

#include "soci.h"

class PLUGIN_DLL_EXPORT SociContainer
	:	public SQLContainer,
		protected Listener<StdString>, // Table changed
		public Listener<SQLPreview::ActionEvent, QVariant, int>	// Handler for the reader config panel when chaning from table to query.
{
public:
	SociContainer(QWidget *oMainWindow);
	virtual ~SociContainer(void);
	soci::session *getSession(void);

	/**
	 * Execute a query and retrieve N rows. If N <= 0 all rows are fetched.
	 * bException returns true if an exception occurs and the result may
	 * be corrupted.
	 */
	std::vector<std::vector<StdString>> fetchRows(std::vector<DatabaseColumn *> &oColumns, StdString const &oQuery, bool &bException, int nCount = 0);
	bool commit(void);
	bool rollback(void);

	/**
	 * Connect to the database. If a session is currently opened
	 * it will be closed. Any uncommited data might be lost.
	 */
	bool connect(StdString const &oConnectString = "") override;
	bool disconnect(StdString const &oConnectString = "") override;

	bool begin(void) override;
	void end(void) override;

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

	virtual int read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow);

	void setSelector(StdString const &oId) override;
	/**
	 * Converts the current selector to a valid query.
	 */
	virtual StdString selectorToQuery(void);

	void setTablename(StdString const &oTableName);	// specifies the tablename if the query is for the while table (select * from x)
	StdString getTablename(void) const;

	void setQuery(StdString const &oQuery);		// Speciefies a custom select.
	StdString getQuery(void) const;

	/**
	 * Reload the tables from the database and update the view. If the
	 * oSelection is empty or doesn't exist, the first table will be selected, otherwise
	 * the specified table.
	 */
	void refreshTables(StdString const &oSelection = "", bool bNotify = true) override;

protected:
	std::vector<DatabaseColumn *> loadColumns(void) override;

	void handleNotification(Dispatcher<StdString> *oSource, StdString oTable);	// Table listener
	void handleNotification(Dispatcher<SQLPreview::ActionEvent, QVariant, int> *oSource, SQLPreview::ActionEvent nEvent, QVariant oData, int nRows);

	virtual bool prepareOpen(std::vector<DatabaseColumn *> const &oColumns);
	virtual int write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow);

	/**
	 * Returns true if the current selector is a table, otherwise it is a query.
	 */
	bool isTable(void);

	/**
	 * Read the column definition from the table data.
	 */
	virtual std::vector<DatabaseColumn *> readColumnsFromTable(StdString const &oTablename);

	/**
	 * Read the column definition from the query result.
	 */
	virtual std::vector<DatabaseColumn *> readColumnsFromQuery(StdString const &oQuery, bool bLimit);
	virtual StdString limitQuery(StdString const &oQuery, int nLimit) const = 0;

	/**
	 * sociConnectString gets the container connect identifier and must convert it to a connectstring
	 * which can be used in soci to connect to the actual database.
	 */
	virtual StdString sociConnectString(StdString const &oContainerConnectString) = 0;
	virtual soci::backend_factory const &sociFactory(void) = 0;
	SQLPreviewPanel *getPreviewPanel(void);
	void initPanel(DBPanel *oPanel) override;
	void refreshPreview(StdString const &oQuery, int nLimit);
	virtual bool isReader(void) = 0;

	virtual std::vector<StdString> loadTables(void) = 0;
	bool createTable(StdString const &oTablename, std::vector<DatabaseColumn *> &oColumns);
	virtual StdString createColumnStatement(DatabaseColumn *oColumn);

	/**
	 * Convert a row into an array of strings
	 */
	std::vector<StdString> fromRow(std::vector<DatabaseColumn *> &oColumns, soci::row const &oRow);

	/**
	 * If the filename is set and bSQLToFile == true the insert statements will also be put into
	 * the specified file.
	 * This should be called before the actual copying is done, to make sure that the
	 * parameters are properly initialized and validated.
	 */
	void setSQLLog(StdString const &SQFilename, bool bSQLToFile, bool bFileOnly);

	/**
	 * Either a file or a db output must be set. Both is allowed. If none
	 * are set, false is returned.
	 */
	bool validateOutput(void);

	/**
	 * Autocommit:
	 * -1: Don't commit and ask the user first.
	 *  0: Autocommit at the end.
	 *  N: Autocommit every N records.
	 *
	 * In case of an error a rollback is performed. If Autocommit N is set, then only the last
	 * inserted records are rollbacked.
	 */
	void setAutocommit(int nAutocommit);

	/**
	 * If ForceCommit = true, then the commit will always be performed.
	 * If set to false, it will only be performed if the autocommit rules apply.
	 *
	 * true is returned if the commit took place.
	 */
	bool commit(bool bForceCommit);

	bool prepareStatement(StdString const &oSelect);

	/**
	 * Convert the input value, to the insertable outputvalue. If false is returned
	 * an error is reported.
	 */
	virtual bool prepareValue(DatabaseColumn *oColumn, StdString const &oValue, StdString &oResult);

	virtual StdString prepareTablename(StdString const &oTablename);
	virtual int count(void);

private:
	typedef SQLContainer super;

	soci::session *mSession;
	soci::rowset<soci::row> *mResultSet;
	soci::rowset<soci::row>::const_iterator mResultIterator;
	SQLPreviewPanel *mPreviewPanel;
	int mPreviewLimit;
	StdString mSociConnectString;
	StdString mQuery;
	StdString mTablename;
	StdString mSQLFilename;
	bool mSQLToFile;		// If true, then the statement will be written to a file.
	bool mFileOnly;			// If false, then the SQL will be inserted into the DB, if true, then it will be written only
							// to the file if mSQLtoFile is also true, otherwise an errorr is thrown.
	int mAutoCommitCount;	// If -1, no autocommit is performed. If 0 then autocommit is done when finished, otherwise every N records a commit is performed.
	int mRecordCount;
	StdString mInsert;
	std::fstream mSQLFile;
};

#endif // _SOCI_CONTAINER_H_INCLUDED
