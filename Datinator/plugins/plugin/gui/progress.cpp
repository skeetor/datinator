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

#include <iostream>

// The application will share a single progress dialog. Doesn't really make sense
// to have individual ones.
QWidget *gMainWindow = NULL;
QProgressDialog *gProgressDlg = NULL;
static int gProgressReference = 0;
supportlib::logging::Logging gLogging;

Progress::Progress(QString const &oTitle, QString const &oText)
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

void Progress::addMessage(supportlib::logging::LoggingItem::LogType nType, QString const &oUnit, QString const &oMessage, unsigned int nTimestamp, QString const &oFile, int nLine, QString const &oMethod)
{
	gLogging.addEntry(oMessage, oUnit, nType, nTimestamp, oFile, nLine, oMethod);
}

void Progress::showProgress(QString const &oText, QString const &oTitle)
{
	gProgressReference++;

	if(gProgressReference == 1)
	{
	    if(!gProgressDlg)
            gProgressDlg = new QProgressDialog(getMainWindow());

	    gProgressDlg->reset();
		gProgressDlg->setWindowModality(Qt::WindowModal);
		supportlib::gui::center(getMainWindow(), gProgressDlg, false, true);
		gProgressDlg->setWindowTitle(oTitle);
		gProgressDlg->show();
        gProgressDlg->setMaximum(1);
        gProgressDlg->setValue(0);
		qApp->processEvents();
	}

	gProgressDlg->setLabelText(oText);
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
