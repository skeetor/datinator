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

class SQLITE_DLL_EXPORT SQLiteReader
	:	public SQLiteContainer
	,	public ReaderBase
{
public:
	SQLiteReader(QWidget *oMainWindow);
	virtual ~SQLiteReader(void);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	int read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow) override;
	int count(void) override;

protected:
	SQLPreviewPanel *createConfigPanel(void) override;
	void initPanel(DBPanel *oPanel) override;
	bool isReader(void) override;
};

#endif // SQLITE_READER_H_INCLUDED
