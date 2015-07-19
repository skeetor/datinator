/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef FILE_CONTAINER_BASE_H_INCLUDED
#define FILE_CONTAINER_BASE_H_INCLUDED

#include "support/patterns/observer.h"
#include "plugin/plugin_dll_api.h"
#include "plugin/container/container_base.h"

class FilePanel;
class FilePanelConfigPanel;

class PLUGIN_DLL_EXPORT FileContainerBase
    : public ContainerBase
    , Listener<QString /* oPath */>
{
public:
	FileContainerBase(QWidget *oMainWindow);
	virtual ~FileContainerBase(void);

	bool connect(QString const &oId) override;
	bool begin(void) override;

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

	/**
	 * For a file container, this will create the main panel which
	 * allows to specify the overall path name. Derived classes
	 * must add their individual configuration panel by overriding
	 * createConfigPanel();
	 */
	virtual QWidget *getConfigPanel(QWidget *oParentPanel) override;

public:
	virtual void setFilename(QString const &oFilename);
	virtual QString getFilename(void);

	virtual bool isUnicode(void);
	virtual void setUnicode(bool bUnicode = true);

protected:
	FilePanel *getFilePanel(void);
	virtual FilePanelConfigPanel *createContainerConfigPanel(QWidget *oParent);
	virtual void enlargePanel(void);
	virtual bool isReader(void) = 0;
	void releaseColumns(void) override;
	void handleNotification(Dispatcher<QString> *oSource, QString oPath) override;

private:
	typedef ContainerBase super;

	bool mUnicode;
	FilePanel *mFilePanel;
};

#endif // FILE_CONTAINER_BASE_H_INCLUDED
