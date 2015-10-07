/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef ORACLE_READER_H_INCLUDED
#define ORACLE_READER_H_INCLUDED

#include "plugin/container/reader_base.h"
#include "oracle/oracle_container.h"

class SQLPreviewPanel;

class ORACLE_DLL_EXPORT OracleReader
	:	public OracleContainer
	,	public ReaderBase
{
public:
	OracleReader(QWidget *oMainWindow);
	virtual ~OracleReader(void);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	int read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow) override;

protected:
	SQLPreviewPanel *createConfigPanel(void) override;
	void initPanel(DBPanel *oPanel) override;
	bool isReader(void) override;
};

#endif // ORACLE_READER_H_INCLUDED
