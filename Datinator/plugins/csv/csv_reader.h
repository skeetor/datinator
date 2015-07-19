/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CSV_READER_H_INCLUDED
#define CSV_READER_H_INCLUDED

#include "plugin/container/reader_base.h"

#include "csv/csv_dll_api.h"
#include "support/db/csv/csv.h"
#include "csv/csv_container.h"
#include "csv/gui/reader/csv_reader_config_panel_gui.moc"

#define CSV_PREVIEW_ROWS		5

class QSettings;
class QWidget;

class CSV_DLL_EXPORT CSVReader
	: public CSVContainer
	, public ReaderBase
{
public:
	CSVReader(QWidget *oMainWindow);
	virtual ~CSVReader(void);

public: // @IDataContainer
	QWidget *getConfigPanel(QWidget *oParentPanel) override;

	int read(QList<DatabaseColumn *> &oColumns, QList<QString> &oRow) override;
	int count(void) override;

protected:
	QList<DatabaseColumn *> loadColumns(void) override;
	CSVReaderConfigPanel *createContainerConfigPanel(QWidget *oParent) override;
	bool isReader(void) override;
	virtual void updateSampleView(QList<DatabaseColumn *> const &oColumns);
	CSV::Openmode getOpenmode(void) override;

private:
	typedef CSVContainer super;

	int mPreviewRowCount;
	CSVReaderConfigPanel *mConfigPanel;
};

#endif // CSV_READER_H_INCLUDED
