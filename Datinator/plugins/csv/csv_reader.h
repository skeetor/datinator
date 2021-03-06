/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _CSV_READER_H_INCLUDED
#define _CSV_READER_H_INCLUDED

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
	int read(std::vector<DatabaseColumn *> &oColumns, std::vector<StdString> &oRow) override;
	int count(void) override;

protected:
	FilePanel *getFilePanel(void) override;
	std::vector<DatabaseColumn *> loadColumns(void) override;
	bool isReader(void) override;
	virtual void updateSampleView(std::vector<DatabaseColumn *> const &oColumns);
	CSV::Openmode getOpenmode(void) override;
	CSVReaderConfigPanel *getReaderConfigPanel(void);

private:
	typedef CSVContainer super;

	int mPreviewRowCount;
	CSVReaderConfigPanel *mConfigPanel;
};

#endif // _CSV_READER_H_INCLUDED
