/*******************************************************************************
 *
 * Datinator SQLite Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __SQLITE_DLL_API_H__
#define __SQLITE_DLL_API_H__

#include <windows.h>

// When building the DLL library, BUILD_SQLITE_DLL must be defined
// in order to export the symbols.


#ifdef BUILD_SQLITE_STATIC
	#define SQLITE_DLL_EXPORT
#else
	#ifdef BUILD_SQLITE_DLL
		#define SQLITE_DLL_EXPORT __declspec(dllexport)
	#else
		#define SQLITE_DLL_EXPORT __declspec(dllimport)
	#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

#endif // __SQLITE_DLL_API_H__
