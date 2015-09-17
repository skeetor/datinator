/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/*******************************************************************************
 * PluginManager is the class which identifies and loads the plugins from the
 * shared libraries.
 *
 ******************************************************************************/

#ifndef PLUGIN_MANAGER_H_INCLUDED
#define PLUGIN_MANAGER_H_INCLUDED

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QPair>

#include "plugin_dll_api.h"

#include "plugin_info.h"

class IDataContainerReader;
class IDataContainerWriter;

class PLUGIN_DLL_EXPORT PluginManager
{
public:
	PluginManager(void);
	virtual ~PluginManager(void);

	/**
	 * Deletes all current readers and writers and rescans the specified
	 * directories for plugin DLLs. Static plugins are not affected.
	 */
	bool reload(void);

	void clearPaths(void);
	void addPath(QString const &oPath);

	/**
	 * true if an error exists.
	 */
	bool hasErrors(void);

	/**
	 * If a function encounters an error, this will return a description.
	 */
	QList<QString> getErrorText(void);
	void clearErrors(void);

	QList<IDataContainerReader *> getReaders(QWidget *oMain);
	QList<IDataContainerWriter *> getWriters(QWidget *oMain);

	/**
	 * Check if there are plugins which have the same UUID.
	 * Returns a list with two pairs which contains the UUID and
	 * the name and path of the duplicate
	 */

	//           UUID                  DLL    Name
	QList<QPair<QString, QList<QPair<QString, QString>>>> findDuplicates(void);

protected:
	void clearReaders(void);
	void clearWriters(void);

	/**
	 * Load a DLL and check if it has a plugin interface. If yes, then
	 * the DLL will be added to the plugin list.
	 */
	bool addDLL(QString const &oPath);

	/**
	 * Scan all directories, including it's subdirectories from mPaths for
	 * DLLs and return them as a list.
	 */
	bool findDLLs(QList<QString> &oDLLs, QString const &oPath);

	/**
	 * Returns number of readers/writers addeed to the list.
	 */
	int addReaders(QList<QFunctionPointer> const &oPluginPointers, QString const &oPath, QWidget *oParent);
	int addWriters(QList<QFunctionPointer> const &oPluginPointers, QString const &oPath, QWidget *oParent);

private:
	QList<PluginInfoReader> mStaticReader;		// List of plugins registered via static call
	QList<PluginInfoWriter> mStaticWriter;		// List of plugins registered via static call
	QList<PluginInfoReader> mDynamicReader;		// List of plugins registered via shared libraries.
	QList<PluginInfoWriter> mDynamicWriter;		// List of plugins registered via shared libraries.

	QList<QString> mPaths;
	QList<QString> mErrors;
};

#endif // PLUGIN_MANAGER_H_INCLUDED