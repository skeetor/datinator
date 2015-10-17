/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _ORACLE_CONTAINER_H_INCLUDED
#define _ORACLE_CONTAINER_H_INCLUDED

#include "datinator_types.h"
#include "oracle/oracle_dll_api.h"
#include "support/patterns/observer.h"

#include "support_qt/db/login_panel/login_event.h"
#include "support_qt/db/login_panel/database_login_panel_gui.moc"
#include "plugin/container/sql/soci_container.h"

class QSettings;
class QWidget;
class DatabaseLoginPanel;
class LoginPanelModel;
namespace soci { class session; }

class ORACLE_DLL_EXPORT OracleContainer
	: public SociContainer
	, protected Listener<LoginEvent>
{
public:
	OracleContainer(QWidget *oMainWindow);
	virtual ~OracleContainer(void);

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool connect(StdString const &oConnectString = "") override;

protected:
	StdString loginToConnectString(DatabaseLogin const &oLogin) const;
	DatabaseLogin connectStringToLogin(StdString const &oConnectString) const;

	StdString limitQuery(StdString const &oQuery, int nLimit) const override;
	virtual std::vector<StdString> loadTables(void) override;

	DatabaseLoginPanel *createLoginPanel(void) override;
	void initPanel(DBPanel *oPanel) override;

	StdString sociConnectString(StdString const &oContainerConnectString) override;
	soci::backend_factory const &sociFactory(void) override;
	std::vector<DatabaseColumn *> readColumnsFromTable(StdString const &oTablename) override;

	void handleNotification(Dispatcher<LoginEvent> *oSource, LoginEvent oEvent) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

private:
	typedef SociContainer super;

	DatabaseLoginPanel *mLoginPanel;
	DatabaseLogin mLogin;
	std::vector<StdString> mTables;
};

#endif // _ORACLE_CONTAINER_H_INCLUDED
