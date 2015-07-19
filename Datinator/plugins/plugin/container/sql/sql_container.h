/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef SQL_CONTAINER_H_INCLUDED
#define SQL_CONTAINER_H_INCLUDED

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

	virtual QWidget *getConfigPanel(QWidget *oParentPanel) override;

	void setQuery(QString const &oQuery);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

	virtual void refreshTables(QString const &oSelection = "", bool bNotify = true) = 0;
	void selectTable(QString const &oSelection, bool bNotify);
	void clearTables(void);

protected:
	virtual QWidget *createLoginPanel(void) = 0;
	virtual QWidget *createConfigPanel(void) = 0;
	virtual void initPanel(DBPanel *oPanel) = 0;

	void setTables(QList<QString> const &oTables, QString const &oSelection = "", bool bNotify = true);

private:
	typedef ContainerBase super;

	DBPanel *mPanel;
	QWidget *mLoginPanel;
	QWidget *mConfigPanel;
};

#endif // SQL_CONTAINER_H_INCLUDED
