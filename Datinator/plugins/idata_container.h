/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/*******************************************************************************
 * IDataContainer is the base interface for a container class which defines the
 * access to read/write data in a database like way.
 *
 * Each container is supposed to handle at least one dataset, but can also
 * handle more. Each dataset is identified by a string that the container uses
 * to address the appropriate dataset.
 * For example a connector to a database could report an arbitrary number because
 * each table would represent an individual dataset. On the other hand, a connector
 * to a CSV file may only handle one dataset,m which is the CSV file.
 ******************************************************************************/

#ifndef _IDATA_CONTAINER_H_INCLUDED
#define _IDATA_CONTAINER_H_INCLUDED

#include <QtCore/QList>
#include <QtCore/QString>

#include "datinator_types.h"

class QSettings;
class QWidget;
class IDataContainerConfigPanel;
class IConfigListener;

template <typename... T> class Listener;

class IDataContainer
{
public:
	IDataContainer(void) {};
	virtual ~IDataContainer(void) {};

	/**
	 * Set the main window for the container. This can be used to
	 * attach the indvidual configuration dialog.
	 */
	virtual void setMainWindow(QWidget *pMainWidget) = 0;

	/**
	 * The column listener will be notified whenever a container
	 * changes it's internal state and thus the list of columns
	 * must be updated. An example would be a database container
	 * where the user chooses a new table.
	 */
	virtual void addColumnListener(Listener<QList<DatabaseColumn *> *> *oColumnListener) = 0;
	virtual void removeColumnListener(Listener<QList<DatabaseColumn *> *> *oColumnListener) = 0;

	/**
	 * The listener will be notified whenever the selector is changed.
	 */
	virtual void addSelectorListener(Listener<QString /* oConnectionString */, QString /* oSelectorId */> *oListener) = 0;
	virtual void removeSelectorListener(Listener<QString, QString> *oListener) = 0;

	/**
	 * Return a unique identifier for this container. Even though this
	 * is currently a simple string, it should be a UUID for possible later
	 * changes.
	 */
	virtual QString getContainerUUID(void) const = 0;

	/**
	 * Return the name of the container. This string should be
	 * in a form that can be presented to the user in GUI so that
	 * he can select the appropriate container.
	 */
	virtual QString getContainername(void) = 0;

	/**
	 * Set currently active selector. Whenever this changes, the
	 * columns must be refreshed.
	 */
	virtual void setSelector(QString const &oId) = 0;
	virtual QString getSelector(void) = 0;

	/**
	 * Return a panel which can be shown to the user for
	 * configuration of the container.
	 */
	virtual QWidget *getConfigPanel(void) = 0;

	/**
	 * Save the state of the container to the property file.
	 */
	virtual void store(QSettings &oPropertyFile, QString const &oPrefix) = 0;
	virtual void restore(QSettings &oPropertyFile, QString const &oPrefix) = 0;

	virtual bool loadProfile(QSettings &oProfile, QString const &oKey) = 0;
	virtual void saveProfile(QSettings &oProfile, QString const &oKey) = 0;

	virtual QList<DatabaseColumn *> getColumns(void) = 0;

	virtual bool connect(QString const &oConnectString = "") = 0;
	virtual bool disconnect(QString const &oConnectString = "") = 0;

	/**
	 * Called when the copying will start.
	 */
	virtual bool begin(void) = 0;

	/**
	 * Called when the copying is done.
	 */
	virtual void end(void) = 0;

	virtual QString getConnectString(void) = 0;
};

#endif // _IDATA_CONTAINER_H_INCLUDED
