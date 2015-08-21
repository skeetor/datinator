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

class SQLiteWriter
	: public SQLiteContainer
	, public WriterBase
{
public:
	SQLiteWriter(QWidget *oMainWindow);
	virtual ~SQLiteWriter(void);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

protected:
	SQLiteWriterPanel *createConfigPanel(void) override;
	bool isReader(void) override;
	/**
	 * Check if the table is conforming to the set of columns, or if it needs to be
	 * updated. If false is returned, and error occured.
	 */
	bool updateTable(QList<DatabaseColumn *> const &oColumns);
	bool toInsertValue(DatabaseColumn const *oColumn, QString const &oValue, QString &oOutValue);

public: // @IDataContainerWriter
	bool canModifyColumns(void) const override;
	bool canTruncate(void) const override;
	bool prepareOpen(QList<DatabaseColumn *>const  &oColumns) override;
	int write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow) override;
	void commit(void) override;
	void rollback(void) override;

private:
	SQLiteWriterPanel *mConfigPanel;
};

#endif // SQLITE_CONTAINER_WRITER_H_INCLUDED
