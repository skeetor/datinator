/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "plugin/container/sql/sql_container.h"
#include "plugin/sql/gui/db_panel_gui.moc"

SQLContainer::SQLContainer(QWidget *oMainWindow)
: ContainerBase(oMainWindow)
{
	mPanel = NULL;
	mLoginPanel = NULL;
	mConfigPanel = NULL;
}

SQLContainer::~SQLContainer(void)
{
}

void SQLContainer::store(QSettings &oPropertyFile, QString const &oPrefix)
{
	ContainerBase::store(oPropertyFile, oPrefix);
}

void SQLContainer::restore(QSettings &oPropertyFile, QString const &oPrefix)
{
	ContainerBase::restore(oPropertyFile, oPrefix);
}

QWidget *SQLContainer::getConfigPanel(void)
{
	if(mPanel == NULL)
	{
#warning ReloadTables implementieren
		mPanel = new DBPanel(super::getMainWindow());
		initPanel(mPanel);
		mLoginPanel = createLoginPanel();
		mConfigPanel = createConfigPanel();
		mPanel->setLoginPanel(mLoginPanel);
		mPanel->setConfigPanel(mConfigPanel);
	}

	return mPanel;
}

void SQLContainer::setTables(QList<QString> const &oTables, QString const &oSelection, bool bNotify)
{
	if(mPanel)
		mPanel->setTables(oTables, oSelection, bNotify);
}

void SQLContainer::selectTable(QString const &oSelection, bool bNotify)
{
	if(mPanel)
		mPanel->selectTable(oSelection, bNotify);
}

void SQLContainer::clearTables(void)
{
	if(mPanel)
		mPanel->setTables(QList<QString>());
}

void SQLContainer::setQuery(QString const &oQuery)
{
	setSelector(oQuery);
}

bool SQLContainer::loadProfile(QSettings &oProfile, QString const &oKey)
{
	if(!super::loadProfile(oProfile, oKey))
		return false;

	if(!mPanel->loadProfile(oProfile, oKey))
		return false;

	return true;
}

void SQLContainer::saveProfile(QSettings &oProfile, QString const &oKey)
{
	super::saveProfile(oProfile, oKey);
	mPanel->saveProfile(oProfile, oKey);
}
