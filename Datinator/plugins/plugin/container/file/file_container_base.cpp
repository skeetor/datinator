/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>

#include "plugin/container/file/file_container_base.h"
#include "plugin/container/gui/file_panel_gui.moc"

FileContainerBase::FileContainerBase(QWidget *oMainWindow)
: super(oMainWindow)
{
	mFilePanel = NULL;
	setUnicode(false);
}

FileContainerBase::~FileContainerBase(void)
{
}

void FileContainerBase::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	oPropertyFile.setValue(oPrefix+"lastpath", getFilename());
}

void FileContainerBase::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
    QString f = oPropertyFile.value(oPrefix+"lastpath", "").toString();
	setFilename(f);
}

FilePanel *FileContainerBase::getFilePanel(void)
{
	return mFilePanel;
}

FilePanelConfigPanel *FileContainerBase::createContainerConfigPanel(QWidget *oParent)
{
	UNUSED(oParent);

	return NULL;
}

QWidget *FileContainerBase::getConfigPanel(QWidget *oParentPanel)
{
	if(mFilePanel == NULL)
	{
		FilePanelConfigPanel *cp = createContainerConfigPanel(NULL);
		mFilePanel = new FilePanel(isReader(), cp, oParentPanel);
		mFilePanel->addPathListener(this);
	}

	return mFilePanel;
}

void FileContainerBase::enlargePanel(void)
{
	if(!mFilePanel)
		return;

	mFilePanel->enlargePanel();
}

bool FileContainerBase::isUnicode(void)
{
	return mUnicode;
}

void FileContainerBase::setUnicode(bool bUnicode)
{
	mUnicode = bUnicode;
}

void FileContainerBase::releaseColumns(void)
{
	super::releaseColumns();
}

void FileContainerBase::handleNotification(Dispatcher<QString> *oSource, QString oPath)
{
	UNUSED(oSource);

    setFilename(oPath);
}

bool FileContainerBase::connect(QString const &oFilename)
{
	getFilePanel()->setFilename(oFilename);
	return super::connect(oFilename);
}

bool FileContainerBase::begin(void)
{
	return super::begin();
}

void FileContainerBase::setFilename(QString const &oFilename)
{
	connect(oFilename);
	setSelector("");
}

QString FileContainerBase::getFilename(void)
{
	return getConnectString();
}

bool FileContainerBase::loadProfile(QSettings &oProfile, QString const &oKey)
{
	return super::loadProfile(oProfile, oKey);
	return true;
}

void FileContainerBase::saveProfile(QSettings &oProfile, QString const &oKey)
{
	super::saveProfile(oProfile, oKey);
}
