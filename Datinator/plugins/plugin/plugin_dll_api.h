/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __PLUGIN_DLL_API_H__
#define __PLUGIN_DLL_API_H__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <support/config.h>

#ifdef BUILD_PLUGINS_STATIC
	#ifndef BUILD_PLUGIN_STATIC
		#define BUILD_PLUGIN_STATIC
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning PLUGIN_STATIC because of PLUGINS dependency
		#endif
	#endif
#else
	#ifndef BUILD_PLUGIN_DLL
		#define BUILD_PLUGIN_DLL
		#ifdef DEBUG_EXPORT_SYMBOLS
			#warning PLUGIN_SHARED because of PLUGINS dependency
		#endif
	#endif
#endif


// When building the DLL library, BUILD_PLUGIN_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_PLUGIN_STATIC
	#define PLUGIN_DLL_EXPORT
	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning PLUGIN_STATIC
	#endif
#else
	#ifdef BUILD_PLUGIN_DLL
        #ifdef _WIN32
            #define PLUGIN_DLL_EXPORT __declspec(dllexport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGIN_SHARED EXPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGIN_DLL_EXPORT __attribute__ ((visibility ("default")))
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGIN_SHARED EXPORT LINUX
			#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define PLUGIN_DLL_EXPORT __declspec(dllimport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGIN_SHARED IMPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGIN_DLL_EXPORT
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning PLUGINS_SHARED IMPORT LINUX
			#endif
        #endif // _LINUX
	#endif // BUILD_PLUGIN_DLL
#endif


#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

#endif // __PLUGIN_DLL_API_H__
