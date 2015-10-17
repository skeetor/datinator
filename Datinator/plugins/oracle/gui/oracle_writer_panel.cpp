/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "plugin/sql/gui/db_export_gui.moc"
#include "plugin/sql/gui/commit_config_gui.moc"
#include "oracle/gui/oracle_writer_gui.moc"
#include "oracle/oracle_writer.h"

OracleWriterPanel::OracleWriterPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::OracleWriterPanelGUI();
	mGUI->setupUi(this);
	mExportPanel = new DBExportPanel();
	mCommitPanel = new CommitConfig();
	int i = 0;
	QGridLayout *cl = mGUI->mConfigLayout;
    cl->addWidget(mExportPanel, i++, 0);
    cl->addWidget(mCommitPanel, i++, 0);
	mCommitPanel->setRowStretch(i, 1);
}

OracleWriterPanel::~OracleWriterPanel(void)
{
}

void OracleWriterPanel::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

void OracleWriterPanel::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

int OracleWriterPanel::getAutoCommitCount(void)
{
	return mCommitPanel->getAutoCommitCount();
}

void OracleWriterPanel::setAutoCommitCount(int nValue)
{
	mCommitPanel->setAutoCommitCount(nValue);
}

bool OracleWriterPanel::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	if(!mCommitPanel->loadProfile(oProfile, oKey))
		return false;

	if(!mExportPanel->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void OracleWriterPanel::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	mCommitPanel->saveProfile(oProfile, oKey);
	mExportPanel->saveProfile(oProfile, oKey);
}

StdString OracleWriterPanel::getPath(void)
{
	return mExportPanel->getPath();
}

void OracleWriterPanel::setPath(StdString const &oPath)
{
	mExportPanel->setPath(oPath);
}

bool OracleWriterPanel::getExportSQL(void)
{
	return mExportPanel->getExportSQL();
}

void OracleWriterPanel::setExportSQL(bool bExportSQL)
{
	mExportPanel->setExportSQL(bExportSQL);
}

bool OracleWriterPanel::getExportSQLOnly(void)
{
	return mExportPanel->getExportSQLOnly();
}

void OracleWriterPanel::setExportSQLOnly(bool bExportSQLOnly)
{
	mExportPanel->setExportSQLOnly(bExportSQLOnly);
}
