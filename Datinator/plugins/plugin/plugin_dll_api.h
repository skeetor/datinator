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

// When building the DLL library, BUILD_PLUGIN_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_PLUGIN_STATIC
	#define PLUGIN_DLL_EXPORT
	#warning PLUGIN_STATIC
#else
	#ifdef BUILD_PLUGIN_DLL
        #ifdef _WIN32
            #define PLUGIN_DLL_EXPORT __declspec(dllexport)
			#warning PLUGIN_SHARED EXPORT WIN32
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGIN_DLL_EXPORT __attribute__ ((visibility ("default")))
			#warning PLUGIN_SHARED EXPORT LINUX
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define PLUGIN_DLL_EXPORT __declspec(dllimport)
			#warning PLUGIN_SHARED IMPORT WIN32
        #endif // _WIN32

        #ifdef _LINUX
            #define PLUGIN_DLL_EXPORT
			#warning PLUGINS_SHARED IMPORT LINUX
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
