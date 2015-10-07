/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef SQLITE_CONTAINER_H_INCLUDED
#define SQLITE_CONTAINER_H_INCLUDED

#include <QtCore/QString>

#include "sqlite/sqlite_dll_api.h"
#include "plugin/container/sql/soci_container.h"

#include "support_qt/file/path_panel/path_panel_gui.moc"
#include "support/patterns/observer.h"

class QSettings;
class QWidget;
class PathSelectPanel;
namespace soci { class session; }

class SQLITE_DLL_EXPORT SQLiteContainer
	: public SociContainer
{
public:
	SQLiteContainer(QWidget *oMainWindow);
	virtual ~SQLiteContainer(void);

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

	virtual QList<QString> loadTables(void) override;

protected:
	bool tableExists(QString oTablename);
	QString limitQuery(QString const &oQuery, int nLimit) const override;

	PathSelectPanel *createLoginPanel(void) override;
	void initPanel(DBPanel *oPanel) override;

	QList<DatabaseColumn *> readColumnsFromTable(QString const &oTablename) override;
	StdString sociConnectString(QString const &oContainerConnectString) override;
	soci::backend_factory const &sociFactory(void) override;

	class PathListener :  public Listener<QString>
	{
		public:
			PathListener(SQLiteContainer *oContainer) { mContainer = oContainer; }
			void handleNotification(Dispatcher<QString> *oSource, QString oPath)
			{
				UNUSED(oSource);

				mContainer->connect(oPath);
			}

		private:
			SQLiteContainer *mContainer;
	};

private:
	typedef SociContainer super;

	PathSelectPanel *mLoginPanel;
	PathListener mPathListener;
};

#endif // SQLITE_CONTAINER_H_INCLUDED
