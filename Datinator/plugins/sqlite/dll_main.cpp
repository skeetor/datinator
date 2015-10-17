/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <vector>

#include "../plugins_dll_api.h"

#include "sqlite/sqlite_global.h"
#include "sqlite/sqlite_dll_api.h"

#include "sqlite/sqlite_reader.h"
#include "sqlite/sqlite_writer.h"

static IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeReader(IDataContainerReader *oReader);

static IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeWriter(IDataContainerWriter *oWriter);

#ifdef BUILD_SQLITE_STATIC
static std::vector<PluginInfo> APIENTRY getPluginInfoFkt(void);

static bool registerPlugins(void)
{
	registerStaticPlugin(getPluginInfoFkt);

	return true;
}

static bool gRegistered = registerPlugins();

#else // BUILD_SQLITE_STATIC

#if defined _WINDOWS

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
    return TRUE; // successful
}
#endif // _WINDOWS

#endif // BUILD_SQLITE_STATIC

#ifdef BUILD_SQLITE_STATIC
static std::vector<PluginInfo> APIENTRY getPluginInfoFkt(void)
#else
extern "C" SQLITE_DLL_EXPORT std::vector<PluginInfo> APIENTRY getPluginInfoFkt(void)
#endif
{
	std::vector<PluginInfo> infos;

	PluginInfoReader r;
	r.setUUID(SQLITE_READER_ID);
	r.setCreatePtr(CreateReader);
	r.setFreePtr(FreeReader);
	infos.push_back(r);

	PluginInfoWriter w;
	w.setUUID(SQLITE_WRITER_ID);
	w.setCreatePtr(CreateWriter);
	w.setFreePtr(FreeWriter);
	infos.push_back(w);

	return infos;
}

static IDataContainerReader *CreateReader(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, SQLITE_READER_ID) != 0)
		return NULL;

	return new SQLiteReader(oMainWindow);
}

static void FreeReader(IDataContainerReader *oReader)
{
	if(oReader)
		delete oReader;
}

static IDataContainerWriter *CreateWriter(const char *oUUID, QWidget *oMainWindow)
{
	// We only support a single reader here, so we can just hardcode it.
	if(!oUUID)
		return NULL;

	if(strcmp(oUUID, SQLITE_WRITER_ID) != 0)
		return NULL;

	return new SQLiteWriter(oMainWindow);
}

static void FreeWriter(IDataContainerWriter *oWriter)
{
	if(oWriter)
		delete oWriter;
}
