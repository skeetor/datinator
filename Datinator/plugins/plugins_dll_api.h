/*******************************************************************************
 *
 * Datinator CSV Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __PLUGINS_DLL_API_H__
#define __PLUGINS_DLL_API_H__

#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <support/config.h>
#include <plugin/plugin_info.h>

class IDataContainerReader;
class IDataContainerWriter;
class QWidget;

// When building the DLL library, BUILD_PLUGINS_AS_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_PLUGINS_AS_STATIC
	#define PLUGINS_EXPORT
	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning PLUGINS_STATIC
	#endif
#else
	#ifdef BUILD_PLUGINS_AS_DLL
        #ifdef _WIN32
            #define PLUGINS_EXPORT __declspec(dllexport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGINS_SHARED EXPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGINS_EXPORT __attribute__ ((visibility ("default")))
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGINS_SHARED EXPORT LINUX
			#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define PLUGINS_EXPORT __declspec(dllimport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGINS_SHARED IMPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGINS_EXPORT
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGINS_SHARED IMPORT LINUX
			#endif
        #endif // _LINUX
	#endif // BUILD_PLUGINS_AS_DLL
#endif

/**
 * A plugin must export the following function(s). When datinator is started
 * it will search it's working directory and any subdirectory for shared libraries.
 * If a library has the
 */

#ifndef _WIN32

#define APIENTRY

#endif // _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Get the list of plugins supported by this shared library.
 */
#ifdef _WIN32
#define getPluginListFkt	"getPluginList@4"
#else
#define getPluginListFkt	"getPluginList"
#endif // _WIN32

extern "C" PLUGINS_EXPORT std::vector<PluginInfo> APIENTRY getPluginList(void);

#ifdef __cplusplus
}
#endif

#endif // __PLUGINS_DLL_API_H__
