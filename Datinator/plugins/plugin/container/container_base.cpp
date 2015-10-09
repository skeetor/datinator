/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>

#include "plugin/container/container_base.h"
#include "support_qt/helper/gui_helper.h"
#include "support_qt/logging/logging_dialog_box_gui.moc"
#include "plugin/gui/progress.h"

ContainerBase::ContainerBase(QWidget *oMainWindow)
{
	mMainWindow = oMainWindow;
}

ContainerBase::~ContainerBase(void)
{
}

void ContainerBase::setMainWindow(QWidget *pMainWidget)
{
	mMainWindow = pMainWidget;
}

TypeString const &ContainerBase::getDatatypeStrings(void)
{
	return TypeStringProvider;
}

QWidget *ContainerBase::getMainWindow(void)
{
	return mMainWindow;
}

void ContainerBase::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

void ContainerBase::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

bool ContainerBase::connect(QString const &oConnectString)
{
	mConnectString = oConnectString;
	return true;
}

bool ContainerBase::disconnect(QString const &oConnectString)
{
	UNUSED(oConnectString);

	return true;
}

bool ContainerBase::begin(void)
{
	return true;
}

void ContainerBase::end(void)
{
}

QString ContainerBase::getConnectString(void)
{
	return mConnectString;
}

void ContainerBase::setConnectString(QString const &oConnectString)
{
	mConnectString = oConnectString;
}

QString ContainerBase::getSelector(void)
{
	return mSelector;
}

void ContainerBase::setSelector(QString const &oId)
{
	releaseColumns();
	mSelector = oId;
	Progress prg("Refresh columns");
	prg->setMaximum(prg->maximum()+1);
	QList<DatabaseColumn *> columns = getColumns();
	prg->setValue(prg->value()+1);
	notifyColumnListeners(&columns);
	notifySelectorListeners(getConnectString(), oId);
}

QList<DatabaseColumn *> ContainerBase::getColumns(void)
{
	if(mColumns.size() == 0)
		mColumns = loadColumns();

	return mColumns;
}

void ContainerBase::releaseColumns(void)
{
	for(DatabaseColumn * &col : mColumns)
		delete col;

	mColumns.clear();
}

void ContainerBase::addSelectorListener(Listener<QString, QString> *oListener)
{
	Dispatcher<QString, QString>::addListener(oListener);
}

void ContainerBase::removeSelectorListener(Listener<QString, QString> *oListener)
{
	Dispatcher<QString, QString>::removeListener(oListener);
}

void ContainerBase::notifySelectorListeners(QString const &oConnectString, QString const &oSelector)
{
	Dispatcher<QString, QString>::notify(oConnectString, oSelector);
}

void ContainerBase::addColumnListener(Listener<QList<DatabaseColumn *> *> *oListener)
{
	Dispatcher<QList<DatabaseColumn *> *>::addListener(oListener);
}

void ContainerBase::removeColumnListener(Listener<QList<DatabaseColumn *> *> *oListener)
{
	Dispatcher<QList<DatabaseColumn *> *>::removeListener(oListener);
}

void ContainerBase::notifyColumnListeners(QList<DatabaseColumn *> *oColumns)
{
	Dispatcher<QList<DatabaseColumn *> *>::notify(oColumns);
}

QString ContainerBase::getContainerUUID(void) const
{
	return mContainerUUID;
}

void ContainerBase::setContainerUUID(QString const &oContainerUUID)
{
	mContainerUUID = oContainerUUID;
}

void ContainerBase::setContainername(QString const &oContainername)
{
	mContainername = oContainername;
}

QString ContainerBase::getContainername(void)
{
	return mContainername;
}

bool ContainerBase::loadProfile(QSettings &oProfile, QString const &oKey)
{
	connect(oProfile.value(oKey+"_connect", "").toString());
	setSelector(oProfile.value(oKey+"_selector", "").toString());

	return true;
}

void ContainerBase::saveProfile(QSettings &oProfile, QString const &oKey)
{
	oProfile.setValue(oKey+"_connect", getConnectString());
	oProfile.setValue(oKey+"_selector", getSelector());
}
