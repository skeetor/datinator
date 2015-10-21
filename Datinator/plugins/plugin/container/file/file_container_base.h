/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef _FILE_CONTAINER_BASE_H_INCLUDED
#define _FILE_CONTAINER_BASE_H_INCLUDED

#include "datinator_types.h"

#include "support/patterns/observer.h"
#include "plugin/plugin_dll_api.h"
#include "plugin/container/container_base.h"

class FilePanel;
class FilePanelConfigPanel;

class PLUGIN_DLL_EXPORT FileContainerBase
    : public ContainerBase
    , Listener<StdString /* oPath */>
{
public:
	FileContainerBase(QWidget *oMainWindow);
	virtual ~FileContainerBase(void);

	bool connect(StdString const &oId) override;
	bool begin(void) override;

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

	QWidget *getConfigPanel(void) override;

public:
	virtual void setFilename(StdString const &oFilename);
	virtual StdString getFilename(void);

	virtual bool isUnicode(void);
	virtual void setUnicode(bool bUnicode = true);

protected:
	/**
	 *  This will create the FilePanel selection. To add custom configuration
	 *  panels, override here and add them as
	 */
	virtual FilePanel *getFilePanel(void);
	virtual bool isReader(void) = 0;
	void releaseColumns(void) override;
	void handleNotification(Dispatcher<StdString> *oSource, StdString oPath) override;

private:
	typedef ContainerBase super;

	bool mUnicode;
	FilePanel *mFilePanel;
};

#endif // _FILE_CONTAINER_BASE_H_INCLUDED
