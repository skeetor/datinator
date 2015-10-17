/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef SQLITE_CONTAINER_WRITER_H_INCLUDED
#define SQLITE_CONTAINER_WRITER_H_INCLUDED

#include "plugin/container/writer_base.h"
#include "sqlite/sqlite_container.h"
#include "sqlite/gui/sqlite_writer_gui.moc"

class SQLITE_DLL_EXPORT SQLiteWriter
	: public SQLiteContainer
	, public WriterBase
{
public:
	SQLiteWriter(QWidget *oMainWindow);
	virtual ~SQLiteWriter(void);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

protected:
	SQLiteWriterPanel *createConfigPanel(void) override;
	bool isReader(void) override;
	/**
	 * Check if the table is conforming to the set of columns, or if it needs to be
	 * updated. If false is returned, and error occured.
	 */
	bool updateTable(std::vector<DatabaseColumn *> const &oColumns);
	bool toInsertValue(DatabaseColumn const *oColumn, StdString const &oValue, StdString &oOutValue);

public: // @IDataContainerWriter
	bool canModifyColumns(void) const override;
	bool canTruncate(void) const override;
	bool prepareOpen(std::vector<DatabaseColumn *>const  &oColumns) override;
	int write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow) override;
	void commit(void) override;
	void rollback(void) override;

private:
	SQLiteWriterPanel *mConfigPanel;
};

#endif // SQLITE_CONTAINER_WRITER_H_INCLUDED
