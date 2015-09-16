/*******************************************************************************
 *
 * Datinator CSV Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __CSV_DLL_API_H__
#define __CSV_DLL_API_H__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <support/config.h>

#include "plugin/plugin_info.h"

// When building the DLL library, BUILD_CSV_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_CSV_STATIC
	#define CSV_DLL_EXPORT
	#define BUILD_CONTAINER_STATIC

	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning CSV_STATIC
	#endif
#else
    #define BUILD_CONTAINER_DLL

	#ifdef BUILD_CSV_DLL
        #ifdef _WIN32
            #define CSV_DLL_EXPORT __declspec(dllexport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED EXPORT WIN32
		#endif
		#endif // _WIN32

        #ifdef _LINUX
            #define CSV_DLL_EXPORT __attribute__ ((visibility ("default")))
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED EXPORT LINUX
		#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define CSV_DLL_EXPORT __declspec(dllimport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED IMPORT WIN32
		#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define CSV_DLL_EXPORT
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED IMPORT LINUX
		#endif
        #endif // _LINUX
	#endif // BUILD_CSV_DLL
#endif


#ifdef __cplusplus
extern "C"
{
#endif

CSV_DLL_EXPORT QList<PluginInfo> APIENTRY getPluginInfo(void);

CSV_DLL_EXPORT IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
CSV_DLL_EXPORT void APIENTRY FreeReader(IDataContainerReader *oReader);

CSV_DLL_EXPORT IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);
CSV_DLL_EXPORT void APIENTRY FreeWriter(IDataContainerWriter *oWriter);


#ifdef __cplusplus
}
#endif

#endif // __CSV_DLL_API_H__
