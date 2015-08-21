/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef SQLITE_READER_H_INCLUDED
#define SQLITE_READER_H_INCLUDED

#include "plugin/container/reader_base.h"
#include "sqlite/sqlite_container.h"

class SQLPreviewPanel;

class SQLiteReader
	:	public SQLiteContainer
	,	public ReaderBase
{
public:
	SQLiteReader(QWidget *oMainWindow);
	virtual ~SQLiteReader(void);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	int read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow) override;
	int count(void) override;

protected:
	SQLPreviewPanel *createConfigPanel(void) override;
	void initPanel(DBPanel *oPanel) override;
	bool isReader(void) override;
};

#endif // SQLITE_READER_H_INCLUDED
