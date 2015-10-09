/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CSV_WRITER_H_INCLUDED
#define CSV_WRITER_H_INCLUDED

#include "plugin/container/writer_base.h"

#include "csv/csv_dll_api.h"
#include "support/db/csv/csv.h"
#include "csv/csv_container.h"
#include "csv/gui/writer/csv_writer_config_panel_gui.moc"

class QSettings;
class QWidget;

class CSV_DLL_EXPORT CSVWriter
	: public CSVContainer
	, public WriterBase
{
public:
	CSVWriter(QWidget *oMainWindow);
	virtual ~CSVWriter(void);

	void setFilename(QString const &oFilename) override;

public: // IDataContainerWriter
	bool canModifyColumns(void) const override;
	bool canTruncate(void) const override;

	bool prepareOpen(QList<DatabaseColumn *> const &oColumns) override;
	int write(QList<DatabaseColumn *> const &oColumns, QList<QString> const &oRow) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

protected:
	CSVWriterConfigPanel *createContainerConfigPanel(void) override;
	QList<DatabaseColumn *> loadColumns(void) override;
	bool isReader(void) override;
	CSV::Openmode getOpenmode(void) override;

private:
	typedef CSVContainer super;

private:
	CSVWriterConfigPanel *mConfigPanel;
};

#endif // CSV_WRITER_H_INCLUDED
