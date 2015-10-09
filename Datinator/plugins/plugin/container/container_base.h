/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef CONTAINER_BASE_H_INCLUDED
#define CONTAINER_BASE_H_INCLUDED

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtWidgets/QProgressDialog>

#include "plugin/plugin_dll_api.h"

#include "idata_container.h"
#include "support/db/dbcolumn.h"
#include "support/patterns/observer.h"
#include "support/logging/logging.h"
#include "plugin/gui/datatype_string.h"

class PLUGIN_DLL_EXPORT ContainerBase
	: public virtual IDataContainer,
	  public Dispatcher<QList<DatabaseColumn *> *>,									// Columns changed
	  public Dispatcher<QString /* oConnectString */, QString /* oSelector */>		// Selector changed.
{
public:
	ContainerBase(QWidget *oMainWindow);
	virtual ~ContainerBase(void);

	static PLUGIN_DLL_EXPORT TypeString const &getDatatypeStrings(void);

	void setMainWindow(QWidget *pMainWidget) override;
	QWidget *getMainWindow(void);

public: // IDataContainer
	QString getContainername(void) override;
	QString getContainerUUID(void) const override;
	void addColumnListener(Listener<QList<DatabaseColumn *> *> *oListener) override;
	void removeColumnListener(Listener<QList<DatabaseColumn *> *> *oListener) override;
	void notifyColumnListeners(QList<DatabaseColumn *> *oColumns);

	void addSelectorListener(Listener<QString, QString> *oListener) override;
	void removeSelectorListener(Listener<QString, QString> *oListener) override;
	void notifySelectorListeners(QString const &oConnectString, QString const &oSelector);

	QList<DatabaseColumn *> getColumns(void) override;

	void setSelector(QString const &oId) override;
	QString getSelector(void) override;

	bool connect(QString const &oConnectString = "") override;
	bool disconnect(QString const &oConnectString = "") override;
	QString getConnectString(void) override;

	bool begin(void) override;
	void end(void) override;

	void store(QSettings &oPropertyFile, QString const &oPrefix) override;
	void restore(QSettings &oPropertyFile, QString const &oPrefix) override;

	// The container base saves/loads the connect string and the selector id.
	bool loadProfile(QSettings &oProfile, QString const &oKey) override;
	void saveProfile(QSettings &oProfile, QString const &oKey) override;

protected:
	virtual void releaseColumns(void);
	virtual QList<DatabaseColumn *> loadColumns(void) = 0;

	virtual void setContainerUUID(QString const &oContainerId);
	virtual void setContainername(QString const &oContainername);
	void setConnectString(QString const &oConnectString);

private:
	QWidget *mMainWindow;
	QList<DatabaseColumn *> mColumns;
	QString mContainerUUID;
	QString mContainername;
	QString mSelector;
	QString mConnectString;
};

#endif // CONTAINER_BASE_H_INCLUDED
