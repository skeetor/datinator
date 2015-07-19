/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "../plugin_dll_api.h"

#include "oracle/oracle_global.h"
#include "oracle/oracle_dll_api.h"

#include "oracle/oracle_reader.h"
#include "oracle/oracle_writer.h"

const char *gReaderIDs[] =
{
	ORACLE_READER_ID,
	NULL
};

const char *gWriterIDs[] =
{
	ORACLE_WRITER_ID,
	NULL
};

extern "C" ORACLE_DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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

	if(strcmp(oUUID, ORACLE_READER_ID) != 0)
		return NULL;

	return new OracleReader(oMainWindow);
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

	if(strcmp(oUUID, ORACLE_WRITER_ID) != 0)
		return NULL;

	return new OracleWriter(oMainWindow);
}

extern "C" PLUGINS_EXPORT void APIENTRY FreeWriter(IDataContainerWriter *oWriter)
{
	if(oWriter)
		delete oWriter;
}
