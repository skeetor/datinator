/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

/*******************************************************************************
 * PluginManager is the class which identifies and loads the plugins from the
 * shared libraries.
 *
 ******************************************************************************/

#include <iostream>		// For debug output to console

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QLibrary>

#include "plugins/plugins_dll_api.h"
#include "plugins/idata_container_reader.h"
#include "plugins/idata_container_writer.h"

#include "plugin_manager.h"

#define MAX_STATIC_PLUGINS		1000

static getPluginListPtr gStaticPlugins[MAX_STATIC_PLUGINS];		// List of plugins registered via static call
static int gStaticPluginsCounter = 0;

PluginManager::PluginManager(void)
{
	std::cout << "Plugins registered:" << gStaticPluginsCounter << std::endl;

	for(int i = 0; i < gStaticPluginsCounter; i++)
	{
		QList<PluginInfo> l = gStaticPlugins[i]();
		for(PluginInfo const &info : l)
		{
			if(info.isReader())
			{
				PluginInfoReader r = info;
				mStaticReader.append(r);
			}
			else
			{
				PluginInfoWriter r = info;
				mStaticWriter.append(r);
			}
		}
	}
}

PluginManager::~PluginManager(void)
{
}

void PluginManager::clearPaths(void)
{
	mPaths.clear();
}

void PluginManager::addPath(QString const &oPath)
{
	mPaths.append(oPath);
}

bool PluginManager::hasErrors(void)
{
	return (mErrors.count() > 0);
}

QList<QString> PluginManager::getErrorText(void)
{
	return mErrors;
}

void PluginManager::clearErrors(void)
{
	mErrors.clear();
}

QList<IDataContainerReader *> PluginManager::getReaders(QWidget *oMain)
{
	QList<IDataContainerReader *> l;
	for(PluginInfoReader &info : mStaticReader)
	{
		IDataContainerReader *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.append(p);
	}
	for(PluginInfoReader &info : mDynamicReader)
	{
		IDataContainerReader *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.append(p);
	}

	return l;
}

QList<IDataContainerWriter *> PluginManager::getWriters(QWidget *oMain)
{
	QList<IDataContainerWriter *> l;
	for(PluginInfoWriter &info : mStaticWriter)
	{
		IDataContainerWriter *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);

		if(p)
			l.append(p);
	}
	for(PluginInfoWriter &info : mDynamicWriter)
	{
		IDataContainerWriter *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.append(p);
	}

	return l;
}

void PluginManager::clearReaders(void)
{
}

void PluginManager::clearWriters(void)
{
}

bool PluginManager::reload(void)
{
	clearErrors();

	clearReaders();
	clearWriters();

	QList<QString> dlls;
	for(QString const &path : mPaths)
		findDLLs(dlls, path);

	std::cout << "DLLs: "<< dlls.size() << "\n" << std::endl;

	for(QString const &path : dlls)
		addDLL(path);

	return true;
}

bool PluginManager::addDLL(QString const &oPath)
{
	std::cout << "Add shared library: "<< oPath.toStdString() << std::endl;
	QLibrary lib(oPath);
	lib.load();
	if(!lib.isLoaded())
	{
		std::cout << "   Not loaded ... \n" << std::endl;
		return false;
	}

	bool unload = true;
	getPluginListPtr getPluginList = reinterpret_cast<getPluginListPtr>(lib.resolve(getPluginListFkt));
	std::cout << "   " << getPluginListFkt << " ... " << std::hex << getPluginList << std::endl;
	if(getPluginList)
	{
		QList<PluginInfo> infos = getPluginList();
		if(infos.size() > 0)
		{
			for(PluginInfo const &info : infos)
			{
				if(info.isReader())
				{
					PluginInfoReader r = info;
					mDynamicReader.append(r);
				}
			}

			unload = false;
		}
	}

	if(unload)
	{
		std::cout << "   Nothing found unloading ... \n" << std::endl;
		lib.unload();
	}
	else
		std::cout << "   Added ... \n" << std::endl;

	return true;
}

/*int PluginManager::addWriters(QList<QFunctionPointer> const &oPluginPointers, QString const &oPath, QWidget *oParent)
{
	int added = 0;

	// All three pointers have to be present
	if(oPluginPointers[PluginFunctionIndex::GetWriterListIndex]
		&& oPluginPointers[PluginFunctionIndex::CreateWriterIndex]
		&& oPluginPointers[PluginFunctionIndex::FreeWriterIndex]
	)
	{
		PluginInfo info;
		PluginGetReaderList getWriterList = reinterpret_cast<PluginGetWriterList>(oPluginPointers[PluginFunctionIndex::GetWriterListIndex]);
		PluginCreateWriter createWriter = reinterpret_cast<PluginCreateWriter>(oPluginPointers[PluginFunctionIndex::CreateWriterIndex]);

		info.setPath(oPath);
		info.setCreatePtr(oPluginPointers[PluginFunctionIndex::CreateWriterIndex]);
		info.setFreePtr(oPluginPointers[PluginFunctionIndex::FreeWriterIndex]);
		const char **uuids = getWriterList();
		if(uuids)
		{
			while(*uuids)
			{
				const char *uuid = *uuids;
				IDataContainerWriter *r = createWriter(uuid, oParent);
				if(r)
				{
					std::cout << "   Writer ... " << uuid << std::endl;
					info.setContainer(r);
					mWriters.append(info);
					added++;
				}
				uuids++;
			}
		}
	}

	return added;
}*/

bool PluginManager::findDLLs(QList<QString> &oDLLs, QString const &oPath)
{
	std::cout << "Scanning: " << oPath.toStdString() << std::endl;
	QDir dir(oPath);
	QFileInfoList entries = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst);
	for(QFileInfo &info : entries)
	{
		QString p = info.canonicalFilePath();

		if(info.isDir())
			findDLLs(oDLLs, p);
		else if(info.isFile())
		{
			QString ext = info.suffix();
			#ifdef _WIN32
			if(ext.toUpper() != "DLL")
				continue;
            #endif // _WIN32

            #ifdef _LINUX
			if(ext.toLower() != "so")
				continue;
            #endif // LINUX

			oDLLs.append(p);
			std::cout << "Found shared library: "<< p.toStdString() << std::endl;
		}
	}

	return true;
}

QList<QPair<QString, QList<QPair<QString, QString>>>> PluginManager::findDuplicates(void)
{
	QList<QPair<QString, QList<QPair<QString, QString>>>> duplist;
	QList<PluginInfo> l;

	for(PluginInfoReader const &info : mStaticReader)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l += i;
	}
	for(PluginInfoReader const &info : mDynamicReader)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l += i;
	}

	for(PluginInfoWriter const &info : mStaticWriter)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l += i;
	}
	for(PluginInfoReader const &info : mDynamicWriter)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l += i;
	}

	for(int i0 = 0; i0 < l.count(); i0++)
	{
		PluginInfo &info = l[i0];
		QString uuid = info.getUUID();

		QList<QPair<QString, QString>> dups;
		QPair<QString, QString> dup;
		dup.first = info.getPath();
		//dup.second = info.getContainer()->getContainername();
		dups.append(dup);

		for(int i1 = 0; i1 < l.count(); i1++)
		{
			if(i0 == i1)
				continue;

			PluginInfo &cmp = l[i1];
			if(uuid == cmp.getUUID())
			{
				dup.first = cmp.getPath();
				//dup.second = cmp.getContainer()->getContainername();
				dups.append(dup);
			}
		}

		if(dups.size() > 1)
		{
			QPair<QString, QList<QPair<QString, QString>>> duplicates;
			duplicates.first = uuid;
			duplicates.second = dups;
			duplist.append(duplicates);
		}
	}

	return duplist;
}

bool registerStaticPlugin(getPluginListPtr pPluginLister)
{
	int max_cnt = sizeof gStaticPlugins / sizeof gStaticPlugins[0];

	if(gStaticPluginsCounter >= max_cnt)
		return false;

	gStaticPlugins[gStaticPluginsCounter++] = pPluginLister;

	return true;
}
