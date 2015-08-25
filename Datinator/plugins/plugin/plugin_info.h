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

typedef IDataContainerReader * APIENTRY (*PluginCreateReader)(const char *oUUID, QWidget *oMainWindow);
typedef void APIENTRY (*PluginFreeReader)(IDataContainerReader *oReader);

typedef IDataContainerWriter * APIENTRY (*PluginCreateWriter)(const char *oUUID, QWidget *oMainWindow);
typedef void APIENTRY (*PluginFreeWriter)(IDataContainerWriter *oWriter);

class PLUGIN_DLL_EXPORT PluginInfo
{
public:
	PluginInfo(void);
	PluginInfo(QString const &oPath);
	PluginInfo(PluginInfo const &oSource);
	virtual ~PluginInfo(void);

	virtual void copy(PluginInfo const &oSource);
	virtual PluginInfo &operator=(PluginInfo const &oSource);

	QString getPath(void) const;
	void setPath(QString const &oPath);

	QString getUUID(void) const { return mUUID; };
	QString getName(void) const { return mName; };

	bool isWriter() const;
	bool isReader() const;
	void setContainer(IDataContainer *oContainer);
	IDataContainer *getContainer(void) const;

	void setCreatePtr(QFunctionPointer oFunctionPointer);
	QFunctionPointer getCreatePtr(void) const;
	void setFreePtr(QFunctionPointer oFunctionPointer);
	QFunctionPointer getFreePtr(void) const;

private:
	QString mPath;
	QString mUUID;
	QString mName;
	IDataContainer *mContainer;
	QFunctionPointer mCreate;
	QFunctionPointer mFree;
};

#endif // PLUGIN_MANAGER_H_INCLUDED
