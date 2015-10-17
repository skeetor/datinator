/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include "support/helper/string.h"

#include "plugin/plugin_dll_api.h"
#include "plugin/sql/gui/db_panel_gui.moc"

DBPanel::DBPanel(QWidget *oParent)
:	QFrame(oParent)
{
	mLoginPanel = NULL;
	mConfigPanel = NULL;

	mGUI = new Ui::DBPanelGUI();
	mGUI->setupUi(this);

	mGUI->mTableBox->setEditable(true);
}

DBPanel::~DBPanel(void)
{
}

void DBPanel::setLoginPanel(QWidget *oLoginPanel)
{
	if(oLoginPanel)
		mGUI->mLoginLayout->addWidget(oLoginPanel);
}

void DBPanel::setConfigPanel(QWidget *oConfigPanel)
{
	if(oConfigPanel)
		mGUI->mConfigLayout->addWidget(oConfigPanel);
}

void DBPanel::setRowStretch(int nRow, int nValue)
{
	mGUI->mMainLayout->setRowStretch(nRow, nValue);
}

void DBPanel::onReloadTables(void)
{
#warning Reload Tables implementieren
//	mContainer->refreshTables();
}

bool DBPanel::selectTable(StdString const &oSelection, bool bNotify)
{
	QComboBox *box = mGUI->mTableBox;
	auto s = spt::string::toQt(oSelection);

	int n = box->count();
	for(int i = 0; i < n; i++)
	{
		if(box->itemText(i) == s)
		{
			box->setCurrentIndex(i);
			if(bNotify)
				onTableSelect(i);

			return true;
		}
	}

	return false;
}

void DBPanel::setTables(std::vector<StdString> const &oTables, StdString const &oSelection, bool bNotify)
{
	QComboBox *box = mGUI->mTableBox;

	box->clear();
	int n = 0;
	int i = -1;
	for(StdString const &tn : oTables)
	{
		i++;
		box->addItem(spt::string::toQt(tn));
		if(tn == oSelection)
			n = i;
	}

	if(box->count() > 0)
		box->setCurrentIndex(n);

	if(bNotify)
		onTableSelect(n);
}

void DBPanel::onTableSelect(int nCurrentSelection)
{
	StdString table;
	if(mGUI->mTableBox->count() > 0)
		table = spt::string::fromQt(mGUI->mTableBox->itemText(nCurrentSelection));

	notifyTableSelectListeners(table);
}

/**
 * If we receive the message that the table box should be enabled, we also send a notification
 * to our listeners, to update their view to the current selection.
 */
void DBPanel::handleNotification(Dispatcher<SQLPreview::ActionEvent, QVariant, int> *oSource, SQLPreview::ActionEvent nEvent, QVariant oData, int nRowCount)
{
	UNUSED(oSource);
	UNUSED(nRowCount);

	bool enable = false;
	if(nEvent == SQLPreview::ActionEvent::EVENT_SQL_TOGGLE)
		enable = !oData.value<bool>();

	mGUI->mTableBox->setEnabled(enable);
	if(enable)
	{
		StdString table = spt::string::fromQt(mGUI->mTableBox->currentText());
		notifyTableSelectListeners(table);
	}
}

bool DBPanel::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto k = spt::string::toQt(oKey);

	mGUI->mTableBox->setCurrentText(oProfile.value(k+"_table", "").toString());
	mGUI->mTableBox->setEnabled(oProfile.value(k+"_table_state", "true").toBool());
	if(mGUI->mTableBox->isEnabled())
	{
		StdString table = spt::string::fromQt(mGUI->mTableBox->currentText());
		notifyTableSelectListeners(table);
	}

	return true;
}

void DBPanel::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	auto k = spt::string::toQt(oKey);

	oProfile.setValue(k+"_table", mGUI->mTableBox->currentText());
	oProfile.setValue(k+"_table_state", mGUI->mTableBox->isEnabled());
}
