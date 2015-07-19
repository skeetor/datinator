/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include "plugin/sql/gui/commit_config_gui.moc"

CommitConfig::CommitConfig(QWidget *oParent)
:	QFrame(oParent)
{
	mGUI = new Ui::CommitConfigGUI();
	mGUI->setupUi(this);
}

CommitConfig::~CommitConfig(void)
{
}

void CommitConfig::setRowStretch(int nIndex, int nStretch)
{
	mGUI->mConfigLayout->setStretch(nIndex, nStretch);
}

void CommitConfig::onConfirmCommit(bool bConfirm)
{
	mGUI->mAutoCommitBox->setChecked(!bConfirm);
	mGUI->mCommitCounterSpin->setEnabled(!bConfirm);
}

void CommitConfig::onAutomaticCommit(bool bAutomatic)
{
	mGUI->mCommitBox->setChecked(!bAutomatic);
	mGUI->mCommitCounterSpin->setEnabled(bAutomatic);
}

int CommitConfig::getAutoCommitCount(void)
{
	if(mGUI->mCommitBox->isChecked())
		return -1;

	return mGUI->mCommitCounterSpin->value();
}

void CommitConfig::setAutoCommitCount(int nValue)
{
	bool chkd = false;

	if(nValue == -1)
	{
		chkd = true;
		nValue = 0;
	}

	mGUI->mCommitBox->setChecked(chkd);
	mGUI->mCommitCounterSpin->setValue(nValue);
}

bool CommitConfig::loadProfile(QSettings &oProfile, QString const &oKey)
{
	mGUI->mCommitBox->setChecked(oProfile.value(oKey+"_commit", "false").toBool());
	mGUI->mAutoCommitBox->setChecked(oProfile.value(oKey+"_auto_commit","true").toBool());
	mGUI->mCommitCounterSpin->setValue(oProfile.value(oKey+"_auto_commit_count", "0").toInt());

	return true;
}

void CommitConfig::saveProfile(QSettings &oProfile, QString const &oKey)
{
	oProfile.setValue(oKey+"_commit", mGUI->mCommitBox->isChecked());
	oProfile.setValue(oKey+"_auto_commit", mGUI->mAutoCommitBox->isChecked());
	oProfile.setValue(oKey+"_auto_commit_count", mGUI->mCommitCounterSpin->value());
}
