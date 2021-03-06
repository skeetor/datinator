/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CSV_CONTAINER_H_INCLUDED
#define CSV_CONTAINER_H_INCLUDED

#include "csv/csv_dll_api.h"
#include "support/db/csv/csv.h"
#include "plugin/container/file/file_container_base.h"

class QSettings;
class QWidget;
class FilePanel;

class CSV_DLL_EXPORT CSVContainer
	: public FileContainerBase
{
public:
	CSVContainer(QWidget *oMainWindow);
	virtual ~CSVContainer(void);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	inline int getRownum(void) { return mRownum; }
	inline void setRownum(int nRownum) { mRownum = nRownum; }

	bool connect(StdString const &oConnectString = "") override;
	bool disconnect(StdString const &oConnectString = "") override;
	bool begin(void) override;
	void end(void) override;

protected:
	virtual CSV &getCSV(void);
	virtual CSV const &getCSV(void) const;

	void releaseColumns(void) override;
	virtual CSV::Openmode getOpenmode(void) = 0;

private:
	typedef FileContainerBase super;

	FilePanel *mFilePanel;
	CSV mCSV;
	int mRownum;
};

#endif // CSV_CONTAINER_H_INCLUDED
