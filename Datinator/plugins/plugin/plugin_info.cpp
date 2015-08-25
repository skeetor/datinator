/*******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 *******************************************************************************/

#include "plugins/idata_container.h"
#include "plugins/idata_container_reader.h"
#include "plugins/idata_container_writer.h"

#include "plugin_info.h"

PluginInfo::PluginInfo(void)
{
}

PluginInfo::PluginInfo(PluginInfo const &oSource)
{
	copy(oSource);
}

PluginInfo::~PluginInfo(void)
{
}

void PluginInfo::copy(PluginInfo const &oSource)
{
	setPath(oSource.getPath());
	setContainer(oSource.getContainer());
}

PluginInfo &PluginInfo::operator=(PluginInfo const &oSource)
{
	copy(oSource);
	return *this;
}

QString PluginInfo::getPath(void) const
{
	return mPath;
}

void PluginInfo::setPath(QString const &oPath)
{
	mPath = oPath;
}

bool PluginInfo::isWriter() const
{
	if(mContainer)
	{
		if(dynamic_cast<IDataContainerWriter *>(mContainer))
			return true;
	}

	return false;
}

bool PluginInfo::isReader() const
{
	if(mContainer)
	{
		if(dynamic_cast<IDataContainerReader *>(mContainer))
			return true;
	}

	return false;
}

void PluginInfo::setContainer(IDataContainer *oContainer)
{
	mContainer = oContainer;
	if(mContainer)
	{
		mUUID = mContainer->getContainerUUID();
		mName = mContainer->getContainername();
	}
	else
	{
		mUUID = "";
		mName = "";
	}
}

IDataContainer *PluginInfo::getContainer(void) const
{
	return mContainer;
}

void PluginInfo::setCreatePtr(QFunctionPointer oFunctionPointer)
{
	mCreate = oFunctionPointer;
}

QFunctionPointer PluginInfo::getCreatePtr(void) const
{
	return mCreate;
}

void PluginInfo::setFreePtr(QFunctionPointer oFunctionPointer)
{
	mFree = oFunctionPointer;
}

QFunctionPointer PluginInfo::getFreePtr(void) const
{
	return mFree;
}
