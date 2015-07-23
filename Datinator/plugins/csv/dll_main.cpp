/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "plugin/plugin_dll_api.h"

#include "csv/csv_global.h"
#include "csv/csv_dll_api.h"

#include "csv/csv_reader.h"
#include "csv/csv_writer.h"

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

#ifdef _WIN32

extern "C" CSV_DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
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
