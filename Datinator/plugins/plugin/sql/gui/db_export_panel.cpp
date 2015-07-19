/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include "plugin/sql/gui/db_export_gui.moc"
#include "support_qt/file/path_panel/path_panel_gui.moc"

DBExportPanel::DBExportPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::DBExportGUI();
	mGUI->setupUi(this);
	mPathPanel = new PathSelectPanel(false, NULL);
	mPathPanel->setEnabled(false);
	mGUI->mPathSelectLayout->addWidget(mPathPanel);
}

DBExportPanel::~DBExportPanel(void)
{
}

bool DBExportPanel::loadProfile(QSettings &oProfile, QString const &oKey)
{
	setPath(oProfile.value(oKey+"_export_sql_file_name", "").toString());
	setExportSQL(oProfile.value(oKey+"_export_sql_file", "false").toBool());
	setExportSQLOnly(oProfile.value(oKey+"_export_sql_file_only", "false").toBool());
	mGUI->mExportSQLOnlyBox->setEnabled(getExportSQL());

	return true;
}

void DBExportPanel::saveProfile(QSettings &oProfile, QString const &oKey)
{
	oProfile.setValue(oKey+"_export_sql_file_name", getPath());
	oProfile.setValue(oKey+"_export_sql_file", getExportSQL());
	oProfile.setValue(oKey+"_export_sql_file_only", getExportSQLOnly());
}

void DBExportPanel::onExportSQL(bool bExportSQL)
{
	mPathPanel->setEnabled(bExportSQL);
	mGUI->mExportSQLOnlyBox->setEnabled(bExportSQL);
}

QString DBExportPanel::getPath(void)
{
	return mPathPanel->getPath();
}

void DBExportPanel::setPath(QString const &oPath)
{
	mPathPanel->setPath(oPath);
}

bool DBExportPanel::getExportSQL(void)
{
	return mGUI->mExportSQLBox->isChecked();
}

void DBExportPanel::setExportSQL(bool bExportSQL)
{
	mGUI->mExportSQLBox->setChecked(bExportSQL);
}

bool DBExportPanel::getExportSQLOnly(void)
{
	return mGUI->mExportSQLOnlyBox->isChecked();
}

void DBExportPanel::setExportSQLOnly(bool bExportSQLOnly)
{
	mGUI->mExportSQLOnlyBox->setChecked(bExportSQLOnly);
}
