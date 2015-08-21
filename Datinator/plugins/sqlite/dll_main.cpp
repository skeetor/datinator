/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "../plugins_dll_api.h"

#include "sqlite/sqlite_global.h"
#include "sqlite/sqlite_dll_api.h"

#include "sqlite/sqlite_reader.h"
#include "sqlite/sqlite_writer.h"

const char *gReaderIDs[] =
{
	SQLITE_READER_ID,
	NULL
};

const char *gWriterIDs[] =
{
	SQLITE_WRITER_ID,
	NULL
};

extern "C" SQLITE_DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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

extern "C" PLUGINS_EXPORT const char ** APIENTRY GetReaderList(void)
{
	return gReaderIDs;
}

extern "C" PLUGINS_EXPORT const char ** APIENTRY GetWriterList(void)
{
	return gWriterIDs;
}

extern "C" PLUGINS_EXPORT IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, SQLITE_READER_ID) != 0)
		return NULL;

	return new SQLiteReader(oMainWindow);
}

extern "C" PLUGINS_EXPORT void APIENTRY FreeReader(IDataContainerReader *oReader)
{
	if(oReader)
		delete oReader;
}

extern "C" PLUGINS_EXPORT IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, SQLITE_WRITER_ID) != 0)
		return NULL;

	return new SQLiteWriter(oMainWindow);
}

extern "C" PLUGINS_EXPORT void APIENTRY FreeWriter(IDataContainerWriter *oWriter)
{
	if(oWriter)
		delete oWriter;
}
