/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "support_qt/file/path_panel/path_panel_gui.moc"
#include "plugin/container/gui/file_panel_gui.moc"
#include "plugin/container/file/file_container_base.h"

FilePanel::FilePanel(bool bReader, QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::FilePanelGUI();
	mGUI->setupUi(this);
	QVBoxLayout *l = mGUI->mMainLayout;
	mPathPanel = new PathSelectPanel(bReader, false, NULL);
	mPathPanel->addPathListener(this);
	l->addWidget(mPathPanel);
}

FilePanel::~FilePanel(void)
{
}

void FilePanel::addConfigPanel(QWidget *pConfigPanel)
{
	mGUI->mMainLayout->addWidget(pConfigPanel);
}

void FilePanel::enlargePanel(void)
{
	mGUI->mMainLayout->setStretch(1, 0);
	mGUI->mDummy->hide();
}

void FilePanel::addPathListener(Listener<StdString> *oListener)
{
	Dispatcher<StdString>::addListener(oListener);
}

void FilePanel::removePathListener(Listener<StdString> *oListener)
{
	Dispatcher<StdString>::removeListener(oListener);
}

void FilePanel::notifyPathListeners(StdString const &oPath)
{
	Dispatcher<StdString>::notify(oPath);
}

void FilePanel::handleNotification(Dispatcher<StdString> *oSource, StdString oPath)
{
	UNUSED(oSource);

    // If the filename is coming via the notification, it means it comes from the
    // GUI and must be propagated to our listeners.
	setFilename(oPath);
	notifyPathListeners(oPath);
}

void FilePanel::setFilename(StdString const &oFilename)
{
	mPathPanel->setPath(oFilename);
}

StdString FilePanel::getFilename(void)
{
	return mPathPanel->getPath();
}

