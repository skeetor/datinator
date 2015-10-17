/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _SQL_CONTAINER_H_INCLUDED
#define _SQL_CONTAINER_H_INCLUDED

#include "datinator_types.h"

#include "plugin/plugin_dll_api.h"
#include "plugin/container/container_base.h"

class DBPanel;
class DBLoginPanel;

class PLUGIN_DLL_EXPORT SQLContainer
	: public ContainerBase
{
public:
	SQLContainer(QWidget *oMainWindow);
	virtual ~SQLContainer(void);

	virtual QWidget *getConfigPanel(void) override;

	void setQuery(StdString const &oQuery);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

	virtual void refreshTables(StdString const &oSelection = "", bool bNotify = true) = 0;
	void selectTable(StdString const &oSelection, bool bNotify);
	void clearTables(void);

protected:
	virtual QWidget *createLoginPanel(void) = 0;
	virtual QWidget *createConfigPanel(void) = 0;
	virtual void initPanel(DBPanel *oPanel) = 0;

	void setTables(std::vector<StdString> const &oTables, StdString const &oSelection = "", bool bNotify = true);

private:
	typedef ContainerBase super;

	DBPanel *mPanel;
	QWidget *mLoginPanel;
	QWidget *mConfigPanel;
};

#endif // _SQL_CONTAINER_H_INCLUDED
