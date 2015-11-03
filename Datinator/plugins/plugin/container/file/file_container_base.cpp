/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>

#include "support/helper/string.h"

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

void FileContainerBase::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	oPropertyFile.setValue(spt::string::toQt(oPrefix)+"lastpath", spt::string::toQt(getFilename()));
}

void FileContainerBase::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
    StdString f = spt::string::fromQt((oPropertyFile.value(spt::string::toQt(oPrefix)+"lastpath", "").toString()));
	setFilename(f);
}

FilePanel *FileContainerBase::getFilePanel(void)
{
	if(mFilePanel == NULL)
	{
		mFilePanel = new FilePanel(isReader(), super::getMainWindow());
		mFilePanel->addPathListener(this);
	}

	return mFilePanel;
}

QWidget *FileContainerBase::getConfigPanel(void)
{
	return getFilePanel();
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

void FileContainerBase::handleNotification(Dispatcher<StdString> *oSource, StdString oPath)
{
	UNUSED(oSource);

    setFilename(oPath);
}

bool FileContainerBase::connect(StdString const &oFilename)
{
	FilePanel *p = getFilePanel();
	if(p)
		p->setFilename(oFilename);

	return super::connect(oFilename);
}

bool FileContainerBase::begin(void)
{
	return super::begin();
}

void FileContainerBase::setFilename(StdString const &oFilename)
{
	connect(oFilename);
	setSelector("");
}

StdString FileContainerBase::getFilename(void)
{
	return getConnectString();
}

bool FileContainerBase::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	return super::loadProfile(oProfile, oKey);
	return true;
}

void FileContainerBase::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	super::saveProfile(oProfile, oKey);
}
