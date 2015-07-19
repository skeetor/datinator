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
#include "plugin/container/gui/file_panel_config_panel.h"

FilePanel::FilePanel(bool bReader, FilePanelConfigPanel *oContainerConfigPanel, QWidget *oParent)
:	QFrame(oParent)
{
	mContainerConfigPanel = oContainerConfigPanel;
	mGUI = new Ui::FilePanelGUI();
	mGUI->setupUi(this);
	QVBoxLayout *l = mGUI->mMainLayout;
	mPathPanel = new PathSelectPanel(bReader, NULL);
	mPathPanel->addPathListener(this);
	l->addWidget(mPathPanel);
	l->addWidget(oContainerConfigPanel);
}

FilePanel::~FilePanel(void)
{
}

void FilePanel::enlargePanel(void)
{
	mGUI->mMainLayout->setStretch(1, 0);
	mGUI->mDummy->hide();
}

void FilePanel::addPathListener(Listener<QString> *oListener)
{
	Dispatcher<QString>::addListener(oListener);
}

void FilePanel::removePathListener(Listener<QString> *oListener)
{
	Dispatcher<QString>::removeListener(oListener);
}

void FilePanel::notifyPathListeners(QString const &oPath)
{
	Dispatcher<QString>::notify(oPath);
}

void FilePanel::handleNotification(Dispatcher<QString> *oSource, QString oPath)
{
	UNUSED(oSource);

    // If the filename is coming via the notification, it means it comes from the
    // GUI and must be propagated to our listeners.
	setFilename(oPath);
	notifyPathListeners(oPath);
}

void FilePanel::setFilename(QString const &oFilename)
{
	mPathPanel->setPath(oFilename);
}

QString FilePanel::getFilename(void)
{
	return mPathPanel->getPath();
}

