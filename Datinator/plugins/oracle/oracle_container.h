/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef ORACLE_CONTAINER_H_INCLUDED
#define ORACLE_CONTAINER_H_INCLUDED

#include "support/patterns/observer.h"

#include "support_qt/db/login_panel/login_event.h"
#include "support_qt/db/login_panel/database_login_panel_gui.moc"
#include "plugin/container/sql/soci_container.h"

class QSettings;
class QWidget;
class DatabaseLoginPanel;
class LoginPanelModel;
namespace soci { class session; }

class OracleContainer : public SociContainer, protected Listener<LoginEvent>
{
public:
	OracleContainer(QWidget *oMainWindow);
	virtual ~OracleContainer(void);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	bool connect(QString const &oConnectString = "") override;

protected:
	QString loginToConnectString(DatabaseLogin const &oLogin) const;
	DatabaseLogin connectStringToLogin(QString const &oConnectString) const;

	QString limitQuery(QString const &oQuery, int nLimit) const override;
	virtual QList<QString> loadTables(void) override;

	DatabaseLoginPanel *createLoginPanel(void) override;
	void initPanel(DBPanel *oPanel) override;

	StdString sociConnectString(QString const &oContainerConnectString) override;
	soci::backend_factory const &sociFactory(void) override;
	QList<DatabaseColumn *> readColumnsFromTable(QString const &oTablename) override;

	void handleNotification(Dispatcher<LoginEvent> *oSource, LoginEvent oEvent) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

private:
	typedef SociContainer super;

	DatabaseLoginPanel *mLoginPanel;
	DatabaseLogin mLogin;
	QList<QString> mTables;
};

#endif // ORACLE_CONTAINER_H_INCLUDED
