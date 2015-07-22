/*******************************************************************************
 *
 * Datinator CSV Plugin (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#ifndef __PLUGINS_DLL_API_H__
#define __PLUGINS_DLL_API_H__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <support/config.h>

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
 * A Plugin DLL can export the following functions. If the DLL supports only one type
 * of items (i.e. only a reader) it doesn't have to export the other symbols.
 *
 * The DLL can be placed anywhere
 * and is  loaded dynamically by searching the executable directory and below.
 * A single plugin DLL can contain multiple readers and writers, but each one
 * has to have a unique UUID to identify it.
 */

#ifndef _WIN32

#define APIENTRY

#endif // _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Return a list of UUIDs for IDataContainerReader. The list is a list of pointers
 * where the last pointer is NULL to indicate the end of the list.
 * A return value of NULL is valid if no items are available.
 */
#ifdef _WIN32
#define GetReaderListName	"GetReaderList@0"
#else
#define GetReaderListName	"GetReaderList"
#endif

PLUGINS_EXPORT const char ** APIENTRY GetReaderList(void);
typedef const char ** APIENTRY (*PluginGetReaderList)(void);

/**
 * Return a list of UUIDs for IDataContainerWriter. The list is a list of pointers
 * where the last pointer is NULL to indicate the end of the list.
 * A return value of NULL is valid if no items are available.
 */
#ifdef _WIN32
#define GetWriterListName	"GetWriterList@0"
#else
#define GetWriterListName	"GetWriterList"
#endif // _WIN32

PLUGINS_EXPORT const char ** APIENTRY GetWriterList(void);
typedef const char ** APIENTRY (*PluginGetWriterList)(void);

/**
 * Create the reader with the specified UUID. If no such reader exists, NULL
 * is returned.
 */
#ifdef _WIN32
#define CreateReaderName	"CreateReader@8"
#define FreeReaderName		"FreeReader@4"
#else
#define CreateReaderName	"CreateReader"
#define FreeReaderName		"FreeReader"
#endif // _WIN32

PLUGINS_EXPORT IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
PLUGINS_EXPORT void APIENTRY FreeReader(IDataContainerReader *oReader);

typedef IDataContainerReader * APIENTRY (*PluginCreateReader)(const char *oUUID, QWidget *oMainWindow);
typedef void APIENTRY (*PluginFreeReader)(IDataContainerReader *oReader);

/**
 * Create the writer with the specified UUID. If no such writer exists, NULL
 * is returned.
 */
#ifdef _WIN32
#define CreateWriterName	"CreateWriter@8"
#define FreeWriterName		"FreeWriter@4"
#else
#define CreateWriterName	"CreateWriter"
#define FreeWriterName		"FreeWriter"
#endif // _WIN32
PLUGINS_EXPORT IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);
PLUGINS_EXPORT void APIENTRY FreeWriter(IDataContainerWriter *oWriter);

typedef IDataContainerWriter * APIENTRY (*PluginCreateWriter)(const char *oUUID, QWidget *oMainWindow);
typedef void APIENTRY (*PluginFreeWriter)(IDataContainerWriter *oWriter);

#ifdef __cplusplus
}
#endif

#endif // __PLUGINS_DLL_API_H__
