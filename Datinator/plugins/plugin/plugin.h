/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CONTAINER_BASE_H_INCLUDED
#define CONTAINER_BASE_H_INCLUDED

#include <vector>
#include <utility>

#include <QtWidgets/QProgressDialog>

#include "datinator_types.h"

#include "idata_container.h"
#include "support/db/dbcolumn.h"
#include "support/patterns/observer.h"
#include "support/logging/logging.h"

class PLUGIN_DLL_EXPORT ContainerBase
	: public virtual IDataContainer,
	  public Dispatcher<std::vector<DatabaseColumn *> *>,	// Columns changed
	  public Dispatcher<StdString /* oConnectString */, StdString /* oSelector */>		// Selector changed.
{
public:
	ContainerBase(QWidget *oMainWindow);
	virtual ~ContainerBase(void);

	static PLUGIN_DLL_EXPORT TypeString const &getDatatypeStrings(void);

	QWidget *getMainWindow(void);

public: // IDataContainer
	StdString getContainername(void) override;
	StdString getContainerUUID(void) const override;
	void addColumnListener(Listener<std::vector<DatabaseColumn *> *> *oListener) override;
	void removeColumnListener(Listener<std::vector<DatabaseColumn *> *> *oListener) override;
	void notifyColumnListeners(std::vector<DatabaseColumn *> *oColumns);

	void addSelectorListener(Listener<StdString, StdString> *oListener) override;
	void removeSelectorListener(Listener<StdString, StdString> *oListener) override;
	void notifySelectorListeners(StdString const &oConnectString, StdString const &oSelector);

	std::vector<DatabaseColumn *> getColumns(void) override;

	void setSelector(StdString const &oId) override;
	StdString getSelector(void) override;

	bool connect(StdString const &oConnectString = "") override;
	bool disconnect(StdString const &oConnectString = "") override;
	StdString getConnectString(void) override;

	bool begin(void) override;
	void end(void) override;

	void store(QSettings &oPropertyFile, StdString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, StdString const &oPrefix) override;

	// The container base saves/loads the connect string and the selector id.
	bool loadProfile(QSettings &oProfile, StdString const &oKey) override;
	void saveProfile(QSettings &oProfile, StdString const &oKey) override;

protected:
	virtual void releaseColumns(void);
	virtual std::vector<DatabaseColumn *> loadColumns(void) = 0;

	virtual void setContainerUUID(StdString const &oContainerId);
	virtual void setContainername(StdString const &oContainername);
	void setConnectString(StdString const &oConnectString);

private:
	QWidget *mMainWindow;
	std::vector<DatabaseColumn *> mColumns;
	StdString mContainerUUID;
	StdString mContainername;
	StdString mSelector;
	StdString mConnectString;
};

#endif // CONTAINER_BASE_H_INCLUDED
