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

// When building the DLL library, BUILD_CSV_DLL must be defined
// in order to export the symbols.

#ifdef BUILD_CSV_STATIC
	#define CSV_DLL_EXPORT
	#define BUILD_CONTAINER_STATIC
#else
    #define BUILD_CONTAINER_DLL

	#ifdef BUILD_CSV_DLL
        #ifdef _WIN32
            #define CSV_DLL_EXPORT __declspec(dllexport)
        #endif // _WIN32

        #ifdef _LINUX
            #define CSV_DLL_EXPORT __attribute__ ((visibility ("default")))
        #endif // _LINUX
	#else
        #ifdef _WIN32
            #define CSV_DLL_EXPORT __declspec(dllimport)
        #endif // _WIN32

        #ifdef _LINUX
            #define CSV_DLL_EXPORT
        #endif // _LINUX
	#endif // BUILD_CSV_DLL
#endif


#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

#endif // __CSV_DLL_API_H__
