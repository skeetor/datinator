/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include "../plugins_dll_api.h"

#include "oracle/oracle_global.h"
#include "oracle/oracle_dll_api.h"

#include "oracle/oracle_reader.h"
#include "oracle/oracle_writer.h"

static IDataContainerReader * APIENTRY CreateReader(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeReader(IDataContainerReader *oReader);

static IDataContainerWriter * APIENTRY CreateWriter(const char *oUUID, QWidget *oMainWindow);
static void APIENTRY FreeWriter(IDataContainerWriter *oWriter);

#ifdef BUILD_ORACLE_STATIC
static QList<PluginInfo> APIENTRY getPluginInfoFkt(void);

static bool registerPlugins(void)
{
	registerStaticPlugin(getPluginInfoFkt);

	return true;
}

static bool gRegistered = registerPlugins();

#else // BUILD_ORACLE_STATIC

#if defined _WINDOWS

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
    return TRUE; // successful
}
#endif // _WINDOWS

#endif // BUILD_ORACLE_STATIC

#ifdef BUILD_ORACLE_STATIC
static QList<PluginInfo> APIENTRY getPluginInfoFkt(void)
#else
extern "C" ORACLE_DLL_EXPORT QList<PluginInfo> APIENTRY getPluginInfoFkt(void)
#endif
{
	QList<PluginInfo> infos;

	PluginInfoReader r;
	r.setUUID(ORACLE_READER_ID);
	r.setCreatePtr(CreateReader);
	r.setFreePtr(FreeReader);
	infos.append(r);

	PluginInfoWriter w;
	w.setUUID(ORACLE_WRITER_ID);
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

	if(strcmp(oUUID, ORACLE_READER_ID) != 0)
		return NULL;

	return new OracleReader(oMainWindow);
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

	if(strcmp(oUUID, ORACLE_WRITER_ID) != 0)
		return NULL;

	return new OracleWriter(oMainWindow);
}

static void FreeWriter(IDataContainerWriter *oWriter)
{
	if(oWriter)
		delete oWriter;
}
