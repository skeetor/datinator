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

#ifdef BUILD_CSV_STATIC

#include "plugin/plugin_manager.h"

static bool registerPlugins(void)
{
	registerStaticPlugin(getPluginInfo);

	return true;
}

static bool gRegistered = registerPlugins();

#endif // BUILD_CSV_STATIC

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
    return TRUE; // successful
}
#endif // _WIN32

extern "C" CSV_DLL_EXPORT QList<PluginInfo> APIENTRY getPluginInfo(void)
{
	QList<PluginInfo> infos;

	PluginInfoReader r;
	r.setUUID(CSV_READER_ID);
	r.setCreatePtr(CreateReader);
	r.setFreePtr(FreeReader);
	infos.append(r);

	PluginInfoWriter w;
	w.setUUID(CSV_WRITER_ID);
	w.setCreatePtr(CreateWriter);
	w.setFreePtr(FreeWriter);
	infos.append(w);

	return infos;
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
