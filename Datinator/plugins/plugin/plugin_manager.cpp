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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QLibrary>

#include "support/helper/string.h"

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
		std::vector<PluginInfo> l = gStaticPlugins[i]();
		for(PluginInfo const &info : l)
		{
			if(info.isReader())
			{
				PluginInfoReader r = info;
				mStaticReader.push_back(r);
			}
			else
			{
				PluginInfoWriter r = info;
				mStaticWriter.push_back(r);
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

void PluginManager::addPath(StdString const &oPath)
{
	mPaths.push_back(oPath);
}

bool PluginManager::hasErrors(void)
{
	return (mErrors.size() > 0);
}

std::vector<StdString> PluginManager::getErrorText(void)
{
	return mErrors;
}

void PluginManager::clearErrors(void)
{
	mErrors.clear();
}

std::vector<IDataContainerReader *> PluginManager::getReaders(QWidget *oMain)
{
	std::vector<IDataContainerReader *> l;
	for(PluginInfoReader &info : mStaticReader)
	{
		IDataContainerReader *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.push_back(p);
	}
	for(PluginInfoReader &info : mDynamicReader)
	{
		IDataContainerReader *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.push_back(p);
	}

	return l;
}

std::vector<IDataContainerWriter *> PluginManager::getWriters(QWidget *oMain)
{
	std::vector<IDataContainerWriter *> l;
	for(PluginInfoWriter &info : mStaticWriter)
	{
		IDataContainerWriter *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);

		if(p)
			l.push_back(p);
	}
	for(PluginInfoWriter &info : mDynamicWriter)
	{
		IDataContainerWriter *p = info.getContainer();
		if(!p)
			p = info.getCreatePtr()(info.getUUID(), oMain);
		if(p)
			l.push_back(p);
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

	std::vector<StdString> dlls;
	for(StdString const &path : mPaths)
		findDLLs(dlls, path);

	std::cout << "DLLs: "<< dlls.size() << "\n" << std::endl;

	for(StdString const &path : dlls)
		addDLL(path);

	return true;
}

bool PluginManager::addDLL(StdString const &oPath)
{
	std::cout << "Add shared library: "<< oPath << std::endl;

	QLibrary lib(spt::string::toQt(oPath));
	lib.load();
	if(!lib.isLoaded())
	{
		std::cout << "   Not loaded ... \n" << std::endl;
		return false;
	}

	bool unload = true;
	getPluginListPtr getPluginList = reinterpret_cast<getPluginListPtr>(lib.resolve(getPluginListFktExport));
	std::cout << "   " << getPluginListFktExport << " ... " << std::hex << getPluginList << std::endl;
	if(getPluginList)
	{
		std::vector<PluginInfo> infos = getPluginList();
		if(infos.size() > 0)
		{
			for(PluginInfo const &info : infos)
			{
				if(info.isReader())
				{
					PluginInfoReader r = info;
					mDynamicReader.push_back(r);
				}
				else
				{
					PluginInfoWriter r = info;
					mDynamicWriter.push_back(r);
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

bool PluginManager::findDLLs(std::vector<StdString> &oDLLs, StdString const &oPath)
{
	std::cout << "Scanning: " << oPath << std::endl;

	QDir dir(spt::string::toQt(oPath));
	QFileInfoList entries = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst);
	for(QFileInfo &info : entries)
	{
		auto p = info.canonicalFilePath();

		if(info.isDir())
			findDLLs(oDLLs, spt::string::fromQt(p));
		else if(info.isFile())
		{
			auto ext = info.suffix();
			#ifdef _WIN32
			if(ext.toUpper() != "DLL")
				continue;
            #endif // _WIN32

            #ifdef _LINUX
			if(ext.toLower() != "so")
				continue;
            #endif // LINUX

			StdString ps = spt::string::fromQt(p);
			oDLLs.push_back(ps);
			std::cout << "Found shared library: "<< ps << std::endl;
		}
	}

	return true;
}

std::vector<std::pair<StdString, std::vector<std::pair<StdString, StdString>>>> PluginManager::findDuplicates(void)
{
	std::vector<std::pair<StdString, std::vector<std::pair<StdString, StdString>>>> duplist;
	std::vector<PluginInfo> l;

	for(PluginInfoReader const &info : mStaticReader)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l.push_back(i);
	}
	for(PluginInfoReader const &info : mDynamicReader)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l.push_back(i);
	}

	for(PluginInfoWriter const &info : mStaticWriter)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l.push_back(i);
	}
	for(PluginInfoReader const &info : mDynamicWriter)
	{
		PluginInfo i;
		i.setUUID(info.getUUID());
		i.setPath(info.getPath());
		l.push_back(i);
	}

	for(size_t i0 = 0; i0 < l.size(); i0++)
	{
		PluginInfo &info = l[i0];
		StdString uuid = info.getUUID();

		std::vector<std::pair<StdString, StdString>> dups;
		std::pair<StdString, StdString> dup;
		dup.first = info.getPath();
		dups.push_back(dup);

		for(size_t i1 = 0; i1 < l.size(); i1++)
		{
			if(i0 == i1)
				continue;

			PluginInfo &cmp = l[i1];
			if(uuid == cmp.getUUID())
			{
				dup.first = cmp.getPath();
				dups.push_back(dup);
			}
		}

		if(dups.size() > 1)
		{
			std::pair<StdString, std::vector<std::pair<StdString, StdString>>> duplicates;
			duplicates.first = uuid;
			duplicates.second = dups;
			duplist.push_back(duplicates);
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
