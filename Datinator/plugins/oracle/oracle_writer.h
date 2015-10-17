/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef ORACLE_WRITER_H_INCLUDED
#define ORACLE_WRITER_H_INCLUDED

#include "plugin/container/writer_base.h"
#include "oracle/oracle_container.h"
#include "oracle/gui/oracle_writer_gui.moc"

class ORACLE_DLL_EXPORT OracleWriter
	: public OracleContainer
	, public WriterBase
{
public:
	OracleWriter(QWidget *oMainWindow);
	virtual ~OracleWriter(void);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

protected:
	OracleWriterPanel *createConfigPanel(void) override;
	StdString getDateFormatString(StdString const &oValue, bool bTimestamp) const;
	bool prepareValue(DatabaseColumn *oColumn, StdString const &oValue, StdString &oResult) override;
	bool isReader(void) override;

public: // @IDataContainerWriter
	bool canModifyColumns(void) const override;
	bool canTruncate(void) const override;
	bool defaultTruncate(void) const override;
	bool prepareOpen(std::vector<DatabaseColumn *> const &oColumns) override;
	int write(std::vector<DatabaseColumn *> const &oColumns, std::vector<StdString> const &oRow) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;
	void commit(void) override;
	void rollback(void) override;

private:
	OracleWriterPanel *mConfigPanel;
	bool mAllowTruncate;
	bool mAllowModifyColumns;
};

#endif // ORACLE_WRITER_H_INCLUDED
