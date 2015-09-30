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

static IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeReader(IDataContainerReader *oReader);

static IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeWriter(IDataContainerWriter *oWriter);

#ifdef BUILD_SQLITE_STATIC
static QList<PluginInfo> APIENTRY getPluginInfo(void);

static bool registerPlugins(void)
{
	registerStaticPlugin(getPluginInfo);

	return true;
}

static bool gRegistered = registerPlugins();

#else // BUILD_SQLITE_STATIC

#if defined _WIN32 || defined _WIN64

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
#endif // _WIN32

#endif // BUILD_SQLITE_STATIC

#ifdef BUILD_SQLITE_STATIC
static QList<PluginInfo> APIENTRY getPluginInfo(void)
#else
extern "C" SQLITE_DLL_EXPORT QList<PluginInfo> APIENTRY getPluginInfo(void)
#endif
{
	QList<PluginInfo> infos;

	PluginInfoReader r;
	r.setUUID(SQLITE_READER_ID);
	r.setCreatePtr(CreateReader);
	r.setFreePtr(FreeReader);
	infos.append(r);

	PluginInfoWriter w;
	w.setUUID(SQLITE_WRITER_ID);
	w.setCreatePtr(CreateWriter);
	w.setFreePtr(FreeWriter);
	infos.append(w);

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
