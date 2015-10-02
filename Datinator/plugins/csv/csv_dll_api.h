/*******************************************************************************
 *
 * Datinator CSV Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __CSV_DLL_API_H__
#define __CSV_DLL_API_H__

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
        #ifdef _WINDOWS
            #define CSV_DLL_EXPORT __declspec(dllexport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED EXPORT WIN32
		#endif
		#endif // _WINDOWS

        #ifdef _LINUX
            #define CSV_DLL_EXPORT __attribute__ ((visibility ("default")))
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED EXPORT LINUX
		#endif
        #endif // _LINUX
	#else
        #ifdef _WINDOWS
            #define CSV_DLL_EXPORT __declspec(dllimport)
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning CSV_SHARED IMPORT WIN32
		#endif
        #endif // _WINDOWS

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

#ifdef BUILD_CSV_DLL
CSV_DLL_EXPORT QList<PluginInfo> APIENTRY getPluginInfo(void);
#endif // BUILD_CSV_STATIC

#ifdef __cplusplus
}
#endif

#endif // __CSV_DLL_API_H__
