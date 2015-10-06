/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QFileDialog>

#include "support_qt/helper/gui_helper.h"
#include "support_qt/file/path_panel/path_panel_gui.moc"

PathSelectPanel::PathSelectPanel(bool bOpenMode, bool bDirectoryMode, QWidget *oParent)
: QFrame(oParent)
{
	mGUI = new Ui::PathSelectGUI();
	mGUI->setupUi(this);
	mGUI->mEncodingBox->addItem("ANSI");
	mGUI->mEncodingBox->addItem("UTF8");
	mGUI->mEncodingBox->addItem("UTF16");
	setDirectoryMode(bDirectoryMode);
	setOpenMode(bOpenMode);
	setEditable(false);
}

PathSelectPanel::~PathSelectPanel(void)
{
}

void PathSelectPanel::setDirectoryMode(bool bDirectoryMode)
{
	mDirectoryMode = bDirectoryMode;
}

bool PathSelectPanel::isDirectoryMode(void)
{
	return mDirectoryMode;
}

void PathSelectPanel::setOpenMode(bool bOpen)
{
	mOpenMode = bOpen;
}

bool PathSelectPanel::isOpenMode(void)
{
	return mOpenMode;
}

void PathSelectPanel::onBrowse(void)
{
	QString fn;

	if(isDirectoryMode())
	{
		fn = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
													getPath(),
		                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	}
	else
	{
		if(isOpenMode())
			fn = QFileDialog::getOpenFileName(this, tr("Open File"), getPath(), tr(""));
		else
			fn = QFileDialog::getSaveFileName(this, tr("Save File"), getPath(), tr(""));
	}

	if(!fn.isNull())
		setPathNotify(fn);
}

void PathSelectPanel::notifyPathListeners(QString const &oPath)
{
	Dispatcher<QString>::notify(oPath);
}

QString PathSelectPanel::getPath(void)
{
	return mGUI->mPathText->text();
}

void PathSelectPanel::setPath(QString const &oPath)
{
	mGUI->mPathText->setText(oPath);
}

void PathSelectPanel::setPathNotify(QString const &oPath)
{
	setPath(oPath);
	notifyPathListeners(oPath);
}

void PathSelectPanel::addPathListener(Listener<QString> *oListener)
{
	Dispatcher<QString>::addListener(oListener);
}

void PathSelectPanel::removePathListener(Listener<QString> *oListener)
{
	Dispatcher<QString>::removeListener(oListener);
}

void PathSelectPanel::setEditable(bool bEditable)
{
	supportlib::gui::setEditable(mGUI->mPathText, bEditable);
}

bool PathSelectPanel::isEditable(void)
{
	return mGUI->mPathText->isReadOnly();
}
