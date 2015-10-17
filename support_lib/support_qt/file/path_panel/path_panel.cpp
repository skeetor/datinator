/*******************************************************************************
 *
 * Support Library QT (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QFileDialog>
#include <QtCore/QString>

#include "support_qt/helper/gui_helper.h"
#include "support_qt/file/path_panel/path_panel_gui.moc"

#include "support/helper/string.h"

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
										spt::string::toQt(getPath()),
										QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	}
	else
	{
		if(isOpenMode())
			fn = QFileDialog::getOpenFileName(this, tr("Open File"), spt::string::toQt(getPath()), tr(""));
		else
			fn = QFileDialog::getSaveFileName(this, tr("Save File"), spt::string::toQt(getPath()), tr(""));
	}

	if(!fn.isNull())
		setPathNotify(spt::string::fromQt(fn));
}

void PathSelectPanel::notifyPathListeners(spt::string::string_t const &oPath)
{
	Dispatcher<spt::string::string_t>::notify(oPath);
}

spt::string::string_t PathSelectPanel::getPath(void)
{
	return spt::string::fromQt(mGUI->mPathText->text());
}

void PathSelectPanel::setPath(spt::string::string_t const &oPath)
{
	mGUI->mPathText->setText(spt::string::toQt(oPath));
}

void PathSelectPanel::setPathNotify(spt::string::string_t const &oPath)
{
	setPath(oPath);
	notifyPathListeners(oPath);
}

void PathSelectPanel::addPathListener(Listener<spt::string::string_t> *oListener)
{
	Dispatcher<spt::string::string_t>::addListener(oListener);
}

void PathSelectPanel::removePathListener(Listener<spt::string::string_t> *oListener)
{
	Dispatcher<spt::string::string_t>::removeListener(oListener);
}

void PathSelectPanel::setEditable(bool bEditable)
{
	spt::gui::setEditable(mGUI->mPathText, bEditable);
}

bool PathSelectPanel::isEditable(void)
{
	return mGUI->mPathText->isReadOnly();
}
