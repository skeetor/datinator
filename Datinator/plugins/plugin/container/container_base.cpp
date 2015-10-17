/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>

#include "support/helper/string.h"

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

void ContainerBase::store(QSettings &oPropertyFile, StdString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

void ContainerBase::restore(QSettings &oPropertyFile, StdString const &oPrefix)
{
	UNUSED(oPropertyFile);
	UNUSED(oPrefix);
}

bool ContainerBase::connect(StdString const &oConnectString)
{
	mConnectString = oConnectString;
	return true;
}

bool ContainerBase::disconnect(StdString const &oConnectString)
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

StdString ContainerBase::getConnectString(void)
{
	return mConnectString;
}

void ContainerBase::setConnectString(StdString const &oConnectString)
{
	mConnectString = oConnectString;
}

StdString ContainerBase::getSelector(void)
{
	return mSelector;
}

void ContainerBase::setSelector(StdString const &oId)
{
	releaseColumns();
	mSelector = oId;
	Progress prg("Refresh columns");
	prg->setMaximum(prg->maximum()+1);
	std::vector<DatabaseColumn *> columns = getColumns();
	prg->setValue(prg->value()+1);
	notifyColumnListeners(&columns);
	notifySelectorListeners(getConnectString(), oId);
}

std::vector<DatabaseColumn *> ContainerBase::getColumns(void)
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

void ContainerBase::addSelectorListener(Listener<StdString, StdString> *oListener)
{
	Dispatcher<StdString, StdString>::addListener(oListener);
}

void ContainerBase::removeSelectorListener(Listener<StdString, StdString> *oListener)
{
	Dispatcher<StdString, StdString>::removeListener(oListener);
}

void ContainerBase::notifySelectorListeners(StdString const &oConnectString, StdString const &oSelector)
{
	Dispatcher<StdString, StdString>::notify(oConnectString, oSelector);
}

void ContainerBase::addColumnListener(Listener<std::vector<DatabaseColumn *> *> *oListener)
{
	Dispatcher<std::vector<DatabaseColumn *> *>::addListener(oListener);
}

void ContainerBase::removeColumnListener(Listener<std::vector<DatabaseColumn *> *> *oListener)
{
	Dispatcher<std::vector<DatabaseColumn *> *>::removeListener(oListener);
}

void ContainerBase::notifyColumnListeners(std::vector<DatabaseColumn *> *oColumns)
{
	Dispatcher<std::vector<DatabaseColumn *> *>::notify(oColumns);
}

StdString ContainerBase::getContainerUUID(void) const
{
	return mContainerUUID;
}

void ContainerBase::setContainerUUID(StdString const &oContainerUUID)
{
	mContainerUUID = oContainerUUID;
}

void ContainerBase::setContainername(StdString const &oContainername)
{
	mContainername = oContainername;
}

StdString ContainerBase::getContainername(void)
{
	return mContainername;
}

bool ContainerBase::loadProfile(QSettings &oProfile, StdString const &oKey)
{
	auto key = spt::string::toQt(oKey);
	connect(spt::string::fromQt(oProfile.value(key+"_connect", "").toString()));
	setSelector(spt::string::fromQt(oProfile.value(key+"_selector", "").toString()));

	return true;
}

void ContainerBase::saveProfile(QSettings &oProfile, StdString const &oKey)
{
	auto key = spt::string::toQt(oKey);
	oProfile.setValue(key+"_connect", spt::string::toQt(getConnectString()));
	oProfile.setValue(key+"_selector", spt::string::toQt(getSelector()));
}
