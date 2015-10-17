/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *
 * MainFrame is the outer main window for the application.
 *
 *******************************************************************************/

#include <QtWidgets/QMessageBox>

#include "gui/main_frame/main_window_gui.moc"
#include "gui/main_frame/most_recent.h"

#include "support/helper/string.h"

#define CURRENT_VERSION "0.5.1"

MainWindow::MainWindow(QWidget *oParent)
:	QMainWindow(oParent)
{
	mGUI = new Ui::MainWindowGUI();
	mGUI->setupUi(this);

	setWindowTitle(spt::string::toQt("Datinator " CURRENT_VERSION " (c) 2014 by Gerhard Gruber"));

	mRecentSource = new MostRecent(mGUI->mRecentSourceMenu);
	mRecentTarget = new MostRecent(mGUI->mRecentTargetMenu);
	mRecentProfile = new MostRecent(mGUI->mRecentProfile);

	auto p = QApplication::applicationDirPath();
	p += "/res/Application.png";
	QIcon i(p);
	setWindowIcon(i);

	mMainFrame = new MainFrame(mRecentSource, mRecentTarget, mRecentProfile, this);
	mGUI->mMainLayout->addWidget(mMainFrame);

	mRestorable.addWidget("main_gui", this);
}

MainWindow::~MainWindow(void)
{
	/*if(mRecentSource)
		delete mRecentSource;

	if(mRecentTarget)
		delete mRecentTarget;*/
}

void MainWindow::store(QSettings &oPropertyfile)
{
	StdString pref = "main_gui/";
	mRestorable.store(oPropertyfile, pref);
	//mRecentSource->store(oPropertyfile, pref);
	//mRecentTarget->store(oPropertyfile, pref);
	mMainFrame->store(oPropertyfile);
}

void MainWindow::restore(QSettings &oPropertyfile)
{
	StdString pref = "main_gui/";
	mRestorable.restore(oPropertyfile, pref);
	//mRecentSource->restore(oPropertyfile, pref);
	//mRecentTarget->restore(oPropertyfile, pref);
	mMainFrame->restore(oPropertyfile);
}

void MainWindow::onExit(void)
{
	close();
}

void MainWindow::onRun(void)
{
	mMainFrame->runProfile();
}

void MainWindow::onLoad(void)
{
	mMainFrame->loadProfile();
}

void MainWindow::onSave(void)
{
	mMainFrame->saveProfile();
}

void MainWindow::reloadPlugins(void)
{
	mMainFrame->reloadPlugins();
}
