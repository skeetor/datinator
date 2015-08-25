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

typedef enum
{
	GetReaderListIndex,
	CreateReaderIndex,
	FreeReaderIndex,
	GetWriterListIndex,
	CreateWriterIndex,
	FreeWriterIndex,
	TableSize
} PluginFunctionIndex;

const char *PluginFunctionNames[] =
{
	GetReaderListName,
	CreateReaderName,
	FreeReaderName,
	GetWriterListName,
	CreateWriterName,
	FreeWriterName,
};

#pragma warning Implement statically registered plugins.
static std::vector<char> gStaticPlugin;

PluginManager::PluginManager(void)
{
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

QList<IDataContainerReader *> PluginManager::getReaders(void)
{
	QList<IDataContainerReader *> l;
	for(PluginInfo &info : mReaders)
	{
		if(info.isReader())
		{
			IDataContainerReader *p = dynamic_cast<IDataContainerReader *>(info.getContainer());
			if(p)
				l.append(p);
		}
	}

	return l;
}

QList<IDataContainerWriter *> PluginManager::getWriters(void)
{
	QList<IDataContainerWriter *> l;
	for(PluginInfo &info : mWriters)
	{
		if(info.isWriter())
		{
			IDataContainerWriter *p = dynamic_cast<IDataContainerWriter *>(info.getContainer());
			if(p)
				l.append(p);
		}
	}

	return l;
}

void PluginManager::clearReaders(void)
{
}

void PluginManager::clearWriters(void)
{
}

bool PluginManager::reload(QWidget *oParent)
{
	clearErrors();

	clearReaders();
	clearWriters();

	QList<QString> dlls;
	for(QString const &path : mPaths)
		findDLLs(dlls, path);

	std::cout << "DLLs: "<< dlls.size() << "\n" << std::endl;

	for(QString const &path : dlls)
		addDLL(path, oParent);

	return true;
}

bool PluginManager::addDLL(QString const &oPath, QWidget *oParent)
{
	std::cout << "Add shared library: "<< oPath.toStdString() << std::endl;
	QLibrary lib(oPath);
	lib.load();
	if(!lib.isLoaded())
	{
		std::cout << "   Not loaded ... \n" << std::endl;
		return false;
	}

	QList<QFunctionPointer> ptrs;
	bool check = false;	// We only need to check if at least one pointer exists
	bool unload = false;
	for(int i = 0; i < PluginFunctionIndex::TableSize; i++)
	{
		QFunctionPointer p = lib.resolve(PluginFunctionNames[i]);
		std::cout << "   " << PluginFunctionNames[i] << " ... " << std::hex << p << std::endl;
		ptrs.append(p);
		if(p)
			check = true;
	}

	if(check)
	{
		addReaders(ptrs, oPath, oParent);
		addWriters(ptrs, oPath, oParent);
	}
	else
		unload = true;

	if(unload)
	{
		std::cout << "   Nothing found unloading ... \n" << std::endl;
		lib.unload();
	}
	else
		std::cout << "   Added ... \n" << std::endl;

	return true;
}

int PluginManager::addReaders(QList<QFunctionPointer> const &oPluginPointers, QString const &oPath, QWidget *oParent)
{
	int added = 0;
	// All three pointers have to be present
	if(oPluginPointers[PluginFunctionIndex::GetReaderListIndex]
		&& oPluginPointers[PluginFunctionIndex::CreateReaderIndex]
		&& oPluginPointers[PluginFunctionIndex::FreeReaderIndex]
	)
	{
		PluginInfo info;
		PluginGetReaderList getReaderList = reinterpret_cast<PluginGetReaderList>(oPluginPointers[PluginFunctionIndex::GetReaderListIndex]);
		PluginCreateReader createReader = reinterpret_cast<PluginCreateReader>(oPluginPointers[PluginFunctionIndex::CreateReaderIndex]);

		info.setPath(oPath);
		info.setCreatePtr(oPluginPointers[PluginFunctionIndex::CreateReaderIndex]);
		info.setFreePtr(oPluginPointers[PluginFunctionIndex::FreeReaderIndex]);
		const char **uuids = getReaderList();
		if(uuids)
		{
			while(*uuids)
			{
				const char *uuid = *uuids;
				IDataContainerReader *r = createReader(uuid, oParent);
				if(r)
				{
					std::cout << "   Reader ... " << uuid << std::endl;
					info.setContainer(r);
					mReaders.append(info);
					added++;
				}
				uuids++;
			}
		}
	}

	return added;
}

int PluginManager::addWriters(QList<QFunctionPointer> const &oPluginPointers, QString const &oPath, QWidget *oParent)
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
}

bool PluginManager::findDLLs(QList<QString> &oDLLs, QString const &oPath)
{
	std::cout << "Scanning: "<< oPath.toStdString() << std::endl;
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
	QList<PluginInfo> l = mReaders;
	l += mWriters;

	for(int i0 = 0; i0 < l.count(); i0++)
	{
		PluginInfo &info = l[i0];
		QString uuid = info.getUUID();

		QList<QPair<QString, QString>> dups;
		QPair<QString, QString> dup;
		dup.first = info.getPath();
		dup.second = info.getContainer()->getContainername();
		dups.append(dup);

		for(int i1 = 0; i1 < l.count(); i1++)
		{
			if(i0 == i1)
				continue;

			PluginInfo &cmp = l[i1];
			if(uuid == cmp.getUUID())
			{
				dup.first = cmp.getPath();
				dup.second = cmp.getContainer()->getContainername();
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

bool PluginManager::registerStaticPluginReader(PluginInfo const &oPluginInfo)
{
	memset(0, 0, 4096);
	std::cout << "Static plugin reader registererd" << std::endl;
	return false;
}

bool PluginManager::registerStaticPluginWriter(PluginInfo const &oPluginInfo)
{
	memset(0, 0, 4096);
	std::cout << "Static plugin reader registererd" << std::endl;
	return false;
}
