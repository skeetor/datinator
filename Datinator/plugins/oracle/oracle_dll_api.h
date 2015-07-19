/*******************************************************************************
 *
 * Datinator Oracle Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __ORACLE_DLL_API_H__
#define __ORACLE_DLL_API_H__

#include <windows.h>

// When building the DLL library, BUILD_ORACLE_DLL must be defined
// in order to export the symbols.


#ifdef BUILD_ORACLE_STATIC
	#define ORACLE_DLL_EXPORT
#else
	#ifdef BUILD_ORACLE_DLL
		#define ORACLE_DLL_EXPORT __declspec(dllexport)
	#else
		#define ORACLE_DLL_EXPORT __declspec(dllimport)
	#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif

#endif // __ORACLE_DLL_API_H__
