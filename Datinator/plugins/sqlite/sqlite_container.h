/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef _SQLITE_CONTAINER_H_INCLUDED
#define _SQLITE_CONTAINER_H_INCLUDED

#define USE_STD_LISTENER

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

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

	virtual std::vector<StdString> loadTables(void) override;

protected:
	bool tableExists(StdString oTablename);
	StdString limitQuery(StdString const &oQuery, int nLimit) const override;

	PathSelectPanel *createLoginPanel(void) override;
	void initPanel(DBPanel *oPanel) override;

	std::vector<DatabaseColumn *> readColumnsFromTable(StdString const &oTablename) override;
	StdString sociConnectString(StdString const &oContainerConnectString) override;
	soci::backend_factory const &sociFactory(void) override;

	class PathListener :  public Listener<StdString>
	{
		public:
			PathListener(SQLiteContainer *oContainer) { mContainer = oContainer; }
			void handleNotification(Dispatcher<StdString> *oSource, StdString oPath)
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
