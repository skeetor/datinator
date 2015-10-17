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

#include <vector>
#include <utility>
#include "datinator_types.h"

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
	void addPath(StdString const &oPath);

	/**
	 * true if an error exists.
	 */
	bool hasErrors(void);

	/**
	 * If a function encounters an error, this will return a description.
	 */
	std::vector<StdString> getErrorText(void);
	void clearErrors(void);

	std::vector<IDataContainerReader *> getReaders(QWidget *oMain);
	std::vector<IDataContainerWriter *> getWriters(QWidget *oMain);

	/**
	 * Check if there are plugins which have the same UUID.
	 * Returns a list with two pairs which contains the UUID and
	 * the name and path of the duplicate
	 */
	std::vector<std::pair<StdString, std::vector<std::pair<StdString, StdString>>>> findDuplicates(void);

protected:
	void clearReaders(void);
	void clearWriters(void);

	/**
	 * Load a DLL and check if it has a plugin interface. If yes, then
	 * the DLL will be added to the plugin list.
	 */
	bool addDLL(StdString const &oPath);

	/**
	 * Scan all directories, including it's subdirectories from mPaths for
	 * DLLs and return them as a list.
	 */
	bool findDLLs(std::vector<StdString> &oDLLs, StdString const &oPath);

	/**
	 * Returns number of readers/writers addeed to the list.
	 */
	int addReaders(std::vector<QFunctionPointer> const &oPluginPointers, StdString const &oPath, QWidget *oParent);
	int addWriters(std::vector<QFunctionPointer> const &oPluginPointers, StdString const &oPath, QWidget *oParent);

private:
	std::vector<PluginInfoReader> mStaticReader;		// List of plugins registered via static call
	std::vector<PluginInfoWriter> mStaticWriter;		// List of plugins registered via static call
	std::vector<PluginInfoReader> mDynamicReader;		// List of plugins registered via shared libraries.
	std::vector<PluginInfoWriter> mDynamicWriter;		// List of plugins registered via shared libraries.

	std::vector<StdString> mPaths;
	std::vector<StdString> mErrors;
};

#endif // PLUGIN_MANAGER_H_INCLUDED
