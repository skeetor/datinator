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
	mContainer = NULL;
	mCreate = NULL;
	mFree = NULL;
}

PluginInfo::PluginInfo(PluginInfo const &oSource)
{
	mContainer = NULL;
	mCreate = NULL;
	mFree = NULL;

	copy(oSource);
}

PluginInfo::~PluginInfo(void)
{
}

void PluginInfo::copy(PluginInfo const &oSource)
{
	setReader(oSource.isReader());
	setPath(oSource.getPath());
	setUUID(oSource.getUUID());
	setCreatePtr(oSource.getCreatePtr());
	setFreePtr(oSource.getFreePtr());

	setContainer(oSource.getContainer());
}

PluginInfo &PluginInfo::operator=(PluginInfo const &oSource)
{
	copy(oSource);
	return *this;
}

bool PluginInfo::isWriter() const
{
	if(mContainer)
	{
		if(dynamic_cast<IDataContainerWriter *>(mContainer))
			return true;
	}
	else
		return !mIsReader;

	return false;
}

void PluginInfo::setReader(bool bIsReader)
{
	mIsReader = bIsReader;
}

bool PluginInfo::isReader() const
{
	if(mContainer)
	{
		if(dynamic_cast<IDataContainerReader *>(mContainer))
			return true;
	}
	else
		return mIsReader;

	return false;
}

void PluginInfo::setContainer(IDataContainer *oContainer)
{
	mContainer = oContainer;
	if(mContainer)
		mName = mContainer->getContainername();
	else
		mName = "";
}
