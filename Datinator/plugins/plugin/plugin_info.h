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

#ifndef PLUGIN_INFO_H_INCLUDED
#define PLUGIN_INFO_H_INCLUDED

#include <QtCore/QString>
#include <QtCore/QFunctionPointer>

#include "plugin_dll_api.h"

class IDataContainer;
class IDataContainerReader;
class IDataContainerWriter;
class QWidget;

extern "C" typedef IDataContainer * APIENTRY (*CreatePtr)(const char *oUUID, QWidget *oMainWindow);
extern "C" typedef void APIENTRY (*FreePtr)(IDataContainer *oContainer);

extern "C" typedef IDataContainerReader * APIENTRY (*CreateReaderPtr)(const char *oUUID, QWidget *oMainWindow);
extern "C" typedef void APIENTRY (*FreeReaderPtr)(IDataContainerReader *oContainer);

extern "C" typedef IDataContainerWriter * APIENTRY (*CreateWriterPtr)(const char *oUUID, QWidget *oMainWindow);
extern "C" typedef void APIENTRY (*FreeWriterPtr)(IDataContainerWriter *oContainer);

class PLUGIN_DLL_EXPORT PluginInfo
{
public:
	PluginInfo(void);
	PluginInfo(QString const &oPath);
	PluginInfo(PluginInfo const &oSource);
	virtual ~PluginInfo(void);

	virtual void copy(PluginInfo const &oSource);
	virtual PluginInfo &operator=(PluginInfo const &oSource);

	inline QString getPath(void) const
	{
		return mPath;
	}
	inline void setPath(QString const &oPath)
	{
		mPath = oPath;
	}

	inline void setUUID(const char *pUUID) { mUUID = pUUID; };
	inline const char *getUUID(void) const { return mUUID; };
	inline QString getName(void) const
	{
		return mName;
	};

	virtual bool isWriter() const;
	virtual bool isReader() const;
	virtual void setReader(bool bIsReader = true);

	void setContainer(IDataContainer *oContainer);
	inline IDataContainer *getContainer(void) const
	{
		return mContainer;
	}

protected:
	inline void setCreatePtr(CreatePtr oFunctionPointer)
	{
		mCreate = oFunctionPointer;
	}
	inline CreatePtr getCreatePtr(void) const
	{
		return mCreate;
	}

	inline void setFreePtr(FreePtr oFunctionPointer)
	{
		mFree = oFunctionPointer;
	}
	inline FreePtr getFreePtr(void) const
	{
		return mFree;
	}

private:
	const char *mUUID;
	QString mPath;
	QString mName;
	bool mIsReader;
	IDataContainer *mContainer;
	CreatePtr mCreate;
	FreePtr mFree;
};

// Helper classes to avoid the nasty casts.
class PLUGIN_DLL_EXPORT PluginInfoReader : virtual public PluginInfo
{
public:
	PluginInfoReader(void) : PluginInfo() { setReader(true); };
	PluginInfoReader(QString const &oPath) : PluginInfo(oPath) { setReader(true); };
	PluginInfoReader(PluginInfoReader const &oSource) : PluginInfo(oSource) {};
	PluginInfoReader(PluginInfo const &oSource) : PluginInfo(oSource) {};
	~PluginInfoReader(void) {};

	inline IDataContainerReader *getContainer(void) const
	{
		return reinterpret_cast<IDataContainerReader *>(super::getContainer());
	}

	inline void setCreatePtr(CreateReaderPtr oFunctionPointer)
	{
		super::setCreatePtr(reinterpret_cast<CreatePtr>(oFunctionPointer));
	}
	inline CreateReaderPtr getCreatePtr(void) const
	{
		return reinterpret_cast<CreateReaderPtr>(super::getCreatePtr());
	}

	inline void setFreePtr(FreeReaderPtr oFunctionPointer)
	{
		super::setFreePtr(reinterpret_cast<FreePtr>(oFunctionPointer));
	}
	inline FreeReaderPtr getFreePtr(void) const
	{
		return reinterpret_cast<FreeReaderPtr>(super::getFreePtr());
	}

	bool isWriter() const override { return false; }
	bool isReader() const override { return true; }

private:
	typedef PluginInfo super;
};

class PLUGIN_DLL_EXPORT PluginInfoWriter : virtual public PluginInfo
{
public:
	PluginInfoWriter(void) : PluginInfo() {setReader(false);};
	PluginInfoWriter(QString const &oPath) : PluginInfo(oPath) {setReader(false);};
	PluginInfoWriter(PluginInfoWriter const &oSource) : PluginInfo(oSource) {};
	PluginInfoWriter(PluginInfo const &oSource) : PluginInfo(oSource) {};
	~PluginInfoWriter(void) {};

	inline IDataContainerWriter *getContainer(void) const
	{
		return reinterpret_cast<IDataContainerWriter *>(super::getContainer());
	}

	inline void setCreatePtr(CreateWriterPtr oFunctionPointer)
	{
		super::setCreatePtr(reinterpret_cast<CreatePtr>(oFunctionPointer));
	}
	inline CreateWriterPtr getCreatePtr(void) const
	{
		return reinterpret_cast<CreateWriterPtr>(super::getCreatePtr());
	}

	inline void setFreePtr(FreeWriterPtr oFunctionPointer)
	{
		super::setFreePtr(reinterpret_cast<FreePtr>(oFunctionPointer));
	}
	inline FreeWriterPtr getFreePtr(void) const
	{
		return reinterpret_cast<FreeWriterPtr>(super::getFreePtr());
	}

	bool isWriter() const override { return true; }
	bool isReader() const override { return false; }

private:
	typedef PluginInfo super;
};

extern "C" typedef QList<PluginInfo> APIENTRY (*getPluginListPtr)(void);
bool registerStaticPlugin(getPluginListPtr pPluginLister);

#endif // PLUGIN_MANAGER_H_INCLUDED
