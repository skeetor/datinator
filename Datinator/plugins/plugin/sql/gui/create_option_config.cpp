/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include <support/support_defs.h>

#include "plugin/sql/gui/create_option_config_gui.moc"

CreateOptionConfig::CreateOptionConfig(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::CreateOptionConfigGUI();
	mGUI->setupUi(this);
}

CreateOptionConfig::~CreateOptionConfig(void)
{
}

void CreateOptionConfig::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
//	oPropertyFile.setValue("csv/"+oPrefix+"lastpath", getContainer()->getFilename());
}

void CreateOptionConfig::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
//	getContainer()->setFilename(oPropertyFile.value("csv/"+oPrefix+"lastpath", "").toString());
}

bool CreateOptionConfig::loadProfile(QSettings &oProfile, QString const &oKey)
{
	setCreateTable(oProfile.value(oKey+"_create_table", "false").toBool());
	setRecreateTable(oProfile.value(oKey+"_recreate_table", "false").toBool());
	setCreateColumns(oProfile.value(oKey+"_create_columns", "false").toBool());

	return true;
}

void CreateOptionConfig::saveProfile(QSettings &oProfile, QString const &oKey)
{
	oProfile.setValue(oKey+"_create_table", getCreateTable());
	oProfile.setValue(oKey+"_recreate_table", getRecreateTable());
	oProfile.setValue(oKey+"_create_columns", getCreateColumns());
}

bool CreateOptionConfig::getCreateTable(void)
{
	return mGUI->mCreateTableBox->isChecked();
}

void CreateOptionConfig::setCreateTable(bool bCreateTable)
{
	return mGUI->mCreateTableBox->setChecked(bCreateTable);
}

bool CreateOptionConfig::getCreateColumns(void)
{
	return mGUI->mCreateColumnsBox->isChecked();
}

void CreateOptionConfig::setCreateColumns(bool bCreateColumns)
{
	mGUI->mCreateColumnsBox->setChecked(bCreateColumns);
}

bool CreateOptionConfig::getRecreateTable(void)
{
	return mGUI->mRecreateTableBox->isChecked();
}

void CreateOptionConfig::setRecreateTable(bool bRecreateTable)
{
	mGUI->mRecreateTableBox->setChecked(bRecreateTable);
}

