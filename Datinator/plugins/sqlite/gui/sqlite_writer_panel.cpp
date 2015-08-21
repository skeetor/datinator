/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "plugin/sql/gui/db_export_gui.moc"
#include "plugin/sql/gui/create_option_config_gui.moc"
#include "plugin/sql/gui/commit_config_gui.moc"
#include "sqlite/sqlite_writer.h"
#include "sqlite/gui/sqlite_writer_gui.moc"

SQLiteWriterPanel::SQLiteWriterPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::SQLiteWriterPanelGUI();
	mGUI->setupUi(this);
	mExportPanel = new DBExportPanel();
	mCreateOptionPanel = new CreateOptionConfig(NULL);
	mCommitPanel = new CommitConfig(NULL);
	int i = 0;
    mGUI->mConfigLayout->addWidget(mExportPanel, i++, 0);
    mGUI->mConfigLayout->addWidget(mCommitPanel, i++, 0);
    mGUI->mConfigLayout->addWidget(mCreateOptionPanel, i++, 0);
}

SQLiteWriterPanel::~SQLiteWriterPanel(void)
{
}

void SQLiteWriterPanel::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

void SQLiteWriterPanel::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

bool SQLiteWriterPanel::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!mCreateOptionPanel->loadProfile(oProfile, oKey))
		return false;

	if(!mExportPanel->loadProfile(oProfile, oKey))
		return false;

	if(!mCommitPanel->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SQLiteWriterPanel::saveProfile(QSettings &oProfile, QString const &oKey)
{
	mExportPanel->saveProfile(oProfile, oKey);
	mCreateOptionPanel->saveProfile(oProfile, oKey);
	mCommitPanel->saveProfile(oProfile, oKey);
}

bool SQLiteWriterPanel::getCreateTable(void)
{
	return mCreateOptionPanel->getCreateTable();
}

void SQLiteWriterPanel::setCreateTable(bool bCreateTable)
{
	mCreateOptionPanel->setCreateTable(bCreateTable);
}

bool SQLiteWriterPanel::getCreateColumns(void)
{
	return mCreateOptionPanel->getCreateColumns();
}

void SQLiteWriterPanel::setCreateColumns(bool bCreateColumns)
{
	mCreateOptionPanel->setCreateColumns(bCreateColumns);
}

bool SQLiteWriterPanel::getRecreateTable(void)
{
	return mCreateOptionPanel->getRecreateTable();
}

void SQLiteWriterPanel::setRecreateTable(bool bRecreateTable)
{
	mCreateOptionPanel->setRecreateTable(bRecreateTable);
}

QString SQLiteWriterPanel::getPath(void)
{
	return mExportPanel->getPath();
}

void SQLiteWriterPanel::setPath(QString const &oPath)
{
	mExportPanel->setPath(oPath);
}

bool SQLiteWriterPanel::getExportSQL(void)
{
	return mExportPanel->getExportSQL();
}

void SQLiteWriterPanel::setExportSQL(bool bExportSQL)
{
	mExportPanel->setExportSQL(bExportSQL);
}

bool SQLiteWriterPanel::getExportSQLOnly(void)
{
	return mExportPanel->getExportSQLOnly();
}

void SQLiteWriterPanel::setExportSQLOnly(bool bExportSQLOnly)
{
	mExportPanel->setExportSQLOnly(bExportSQLOnly);
}

int SQLiteWriterPanel::getAutoCommitCount(void)
{
	return mCommitPanel->getAutoCommitCount();
}

void SQLiteWriterPanel::setAutoCommitCount(int nValue)
{
	mCommitPanel->setAutoCommitCount(nValue);
}
