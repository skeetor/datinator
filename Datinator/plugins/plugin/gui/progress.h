/******************************************************************************
 *
 * Datinator (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#ifndef PROGRESS_H_INCLUDED
#define PROGRESS_H_INCLUDED

#include "plugin/plugin_dll_api.h"
#include <support/logging/logging.h>

#define ErrorMessage(t, u, m) Progress::addMessage((t), (u), (m), 0, __FILE__, __LINE__, __func__)

class QProgressDialog;

class PLUGIN_DLL_EXPORT Progress
{
public:
	Progress(QString const &oText, QString const &oTitle = "");
	virtual ~Progress(void);
	QProgressDialog *operator->(void);

	void showProgress(QString const &oText, QString const &oTitle = "");
	void closeProgress(void);

	static void PLUGIN_DLL_EXPORT addMessage(supportlib::logging::LoggingItem::LogType nType, QString const &oUnit, QString const &oMessage, unsigned int nTimestamp, QString const &oFile, int nLine, QString const &oMethod);
	void showMessages(void);
	void clearMessages(void);
	bool hasMessages(void);

	static void PLUGIN_DLL_EXPORT setParent(QWidget *oMainWindow);

protected:
	QWidget *getMainWindow(void);
};

#endif // CONTAINER_BASE_H_INCLUDED
