/*******************************************************************************
 *
 * Support Library (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __SUPPORT_QT_DLL_API_H__
#define __SUPPORT_QT_DLL_API_H__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <support/config.h>

// When building the DLL library, BUILD_SUPPORT_QT_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_SUPPORT_QT_STATIC
	#define SUPPORT_QT_DLL_EXPORT
	#ifdef DEBUG_EXPORT_SYMBOLS
		#warning SUPPORT_QT_STATIC
	#endif
#else
	#ifdef BUILD_SUPPORT_QT_DLL
        #ifdef _WIN32
            #define SUPPORT_QT_DLL_EXPORT __declspec(dllexport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning SUPPORT_QT_SHARED EXPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define SUPPORT_QT_DLL_EXPORT __attribute__ ((visibility ("default")))
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning SUPPORT_QT_SHARED EXPORT LINUX
			#endif
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define SUPPORT_QT_DLL_EXPORT __declspec(dllimport)
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning SUPPORT_QT_SHARED IMPORT WIN32
			#endif
        #endif // _WIN32

        #ifdef _LINUX
            #define SUPPORT_QT_DLL_EXPORT
			#ifdef DEBUG_EXPORT_SYMBOLS
				#warning SUPPORT_QT_SHARED IMPORT LINUX
			#endif
        #endif // _LINUX
	#endif // BUILD_SUPPORT_QT_DLL
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif // __SUPPORT_QT_DLL_API_H__
