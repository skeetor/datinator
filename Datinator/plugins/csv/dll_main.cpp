/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "../plugins_dll_api.h"

#include "csv/csv_global.h"
#include "csv/csv_dll_api.h"

#include "csv/csv_reader.h"
#include "csv/csv_writer.h"

extern "C" CSV_DLL_EXPORT IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
extern "C" CSV_DLL_EXPORT IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);


const char *gReaderIDs[] =
{
	CSV_READER_ID,
	NULL
};

const char *gWriterIDs[] =
{
	CSV_WRITER_ID,
	NULL
};

#ifdef BUILD_CSV_STATIC

#include "plugin/plugin_manager.h"

class StaticCSV
{
public:
	static bool mStaticInitialized;
	static bool _init()
	{
		memset(0, 0, 4096);

		// The manager will create an object with the Create* functions
		// and from this it will get the UUID and other info, so we don't
		// need to set this here (and even can't).
		// Path stays empty as this is a statically compiled version anyway.
		PluginInfo pi;
		pi.setCreatePtr(reinterpret_cast<QFunctionPointer>(CreateReader));
		pi.setFreePtr(reinterpret_cast<QFunctionPointer>(FreeReader));
		PluginManager::registerStaticPluginReader(pi);

		pi.setCreatePtr(reinterpret_cast<QFunctionPointer>(CreateWriter));
		pi.setFreePtr(reinterpret_cast<QFunctionPointer>(FreeWriter));
		PluginManager::registerStaticPluginWriter(pi);

		return true;
	}
};

bool StaticCSV::mStaticInitialized = StaticCSV::_init();

#endif


#ifdef _WIN32

extern "C" CSV_DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	UNUSED(hinstDLL);
	UNUSED(lpvReserved);

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
		break;

        case DLL_PROCESS_DETACH:
            // detach from process
           break;

        case DLL_THREAD_ATTACH:
            // attach to thread
           break;

        case DLL_THREAD_DETACH:
            // detach from thread
           break;
    }
    return TRUE; // succesful
}
#endif // _WIN32

extern "C" CSV_DLL_EXPORT const char ** APIENTRY GetReaderList(void)
{
	return gReaderIDs;
}

extern "C" CSV_DLL_EXPORT const char ** APIENTRY GetWriterList(void)
{
	return gWriterIDs;
}

extern "C" CSV_DLL_EXPORT IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, CSV_READER_ID) != 0)
		return NULL;

	return new CSVReader(oMainWindow);
}

extern "C" CSV_DLL_EXPORT void APIENTRY FreeReader(IDataContainerReader *oReader)
{
	if(oReader)
		delete oReader;
}

extern "C" CSV_DLL_EXPORT IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, CSV_WRITER_ID) != 0)
		return NULL;

	return new CSVWriter(oMainWindow);
}

extern "C" CSV_DLL_EXPORT void APIENTRY FreeWriter(IDataContainerWriter *oWriter)
{
	if(oWriter)
		delete oWriter;
}
