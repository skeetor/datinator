/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

//#warning debug
//#include <iostream>

#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QMessageBox>

#include "support_qt/helper/gui_helper.h"
#include "support_qt/logging/logging_dialog_box_gui.moc"
#include "plugin/gui/progress.h"
#include "support/helper/string.h"

// The application will share a single progress dialog. Doesn't really make sense
// to have individual ones.
QWidget *gMainWindow = NULL;
QProgressDialog *gProgressDlg = NULL;
static int gProgressReference = 0;
spt::logging::Logging gLogging;

Progress::Progress(StdString const &oTitle, StdString const &oText)
{
	showProgress(oText, oTitle);
}

Progress::~Progress(void)
{
	closeProgress();
}

QProgressDialog *Progress::operator->(void)
{
	return gProgressDlg;
}

void Progress::setParent(QWidget *oMainWindow)
{
	gMainWindow = oMainWindow;
}

QWidget *Progress::getMainWindow(void)
{
	return gMainWindow;
}

void Progress::clearMessages(void)
{
	gLogging.clear();
}

void Progress::showMessages(void)
{
	if(gLogging.count() > 0)
	{
		LoggingDialogBox msgBox(getMainWindow());

		msgBox.setLog(gLogging);
		msgBox.exec();
		clearMessages();
	}
}

void Progress::addMessage(spt::logging::LoggingItem::LogType nType, StdString const &oUnit, StdString const &oMessage, unsigned int nTimestamp, StdString const &oFile, int nLine, StdString const &oMethod)
{
	gLogging.addEntry(oMessage, oUnit, nType, nTimestamp, oFile, nLine, oMethod);
}

void Progress::showProgress(StdString const &oText, StdString const &oTitle)
{
	gProgressReference++;

	if(gProgressReference == 1)
	{
	    if(!gProgressDlg)
            gProgressDlg = new QProgressDialog(getMainWindow());

	    gProgressDlg->reset();
		gProgressDlg->setWindowModality(Qt::WindowModal);
		spt::gui::center(getMainWindow(), gProgressDlg, false, true);
		gProgressDlg->setWindowTitle(spt::string::toQt(oTitle));
		gProgressDlg->show();
        gProgressDlg->setMaximum(1);
        gProgressDlg->setValue(0);
		qApp->processEvents();
	}

	gProgressDlg->setLabelText(spt::string::toQt(oText));
}

void Progress::closeProgress(void)
{
	if(gProgressReference)
    {
		gProgressReference--;

        if(gProgressReference == 0)
        {
            if(gProgressDlg)
                gProgressDlg->hide();

            showMessages();
        }
    }
}

bool Progress::hasMessages(void)
{
	if(gLogging.count() > 0)
		return true;

	return false;
}
